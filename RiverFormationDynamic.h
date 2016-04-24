#ifndef RIVER_FORMATION_DYNAMICS_H
#define RIVER_FORMATION_DYNAMICS_H

#include <memory>
#include <valarray>
#include "Vertex.h"
#include <random>
#include <chrono>
#include <numeric>
#include <string>
#include <fstream>
#include "DrawGraph.h"

class RiverFormationDynamic
{
public:
	RiverFormationDynamic(const std::string& graphName)
	{
		FILE* file;
		fopen_s(&file, graphName.c_str(), "r");
		if (file != nullptr)
		{
			fscanf(file, "%d", &graphSize);
			graph.reset(new Vertex[graphSize]);
			int indx;
			float x, y;
			for (int i = 0; i < graphSize; ++i)
			{
				fscanf(file, "%d %f %f", &indx, &x, &y);
				graph[indx].index = indx;
				graph[indx].position = glm::vec2(x, y);
			}
			int s, d, w;
			while (fscanf(file, "%d %d %d", &s, &d, &w) != EOF)
			{
				graph[s].index = s;
				graph[d].index = d;
				graph[s].neighbors.push_back(Pair(d, w));
				graph[d].neighbors.push_back(Pair(s, w));
			}
			//print graph
			/*printf("graph size = %d\n", graphSize);
			for (int i = 0; i < graphSize; ++i)
			{
				printf("vertex index = %d\n ", graph[i].index);
				printf("vertex position =  (%f,%f)\n", graph[i].position.x, graph[i].position.y);
				printf("neighbors: ");
				for (int j = 0; j < graph[i].neighbors.size(); ++j)
					printf("%d %lf", graph[i].neighbors[j].index, graph[i].neighbors[j].distance);
				printf("\n");
			}*/
			//std::cout << graphSize << "\n";
			//std::cin.get();
			drops.resize(graphSize / 4);
			for (int i = 0; i < graphSize; ++i)
				vertexs.push_back(graph[i]);
			drawGraph.reset(new DrawGraph<Vertex>(std::move(vertexs)));
		}
		else
		{
			std::cout << "Cannot open file " << graphName << "\n";
		}
	}
public:
	void run()
	{
		end = false;
		int t = 0;
		initializeGraph();
		while (!end)
		{
			std::cout << "start nr " << t << "\n";
			++t;
			//std::cin.get();
			std::this_thread::sleep_for(std::chrono::seconds(2));
			through();
			drawGraph->draw(bestPath);
		}
		SDL_Event e;
		bool quit = false;
		while (!quit)
		{
			if (SDL_PollEvent(&e))
				if (e.type == SDL_QUIT)
					quit = true;
			drawGraph->draw(bestPath);
		}
	}
private:
	void initializeGraph()
	{
		for (int i = 0; i < graphSize-1; ++i)
			graph[i].high = 255;
		graph[graphSize - 1].high = 0;
	}
	
	void initializeDrops()
	{
		for (int i = 0; i < drops.size(); ++i)
			drops[i].reset(0);
	}

	void through()
	{
		initializeDrops();
		for (int i = 0; i < drops.size(); ++i)
		{
			std::cout << "drop nr " << i << "\n\n";
			Drop& drop = drops[i];
			drop.path.push_back(drop.actualVertex);
			//std::cout << "take drop\n";
			while (drop.actualVertex != graph[graphSize - 1].index)
			{
				//std::cout << "in loop\n";
				//std::cin.get();
				double sumProp = 0;
				std::vector<std::pair<int, double>> p;
				for (int j = 0; j < graph[drop.actualVertex].neighbors.size(); ++j)
				{
					int k = graph[drop.actualVertex].neighbors[j].index;
					double temp = propability(drop.actualVertex, j);
					p.push_back(std::make_pair(j, temp));
					sumProp += temp;
				}
				//std::cout << "tu sie syplem\n";
				if (sumProp > 0)
				{
					for (auto& prop : p)
						prop.second /= sumProp;
				}
				std::shuffle(std::begin(p), std::end(p), std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
				std::sort(std::begin(p), std::end(p), [](const std::pair<int, double>& p1, const std::pair<int, double>& p2) { return p2.second < p1.second; });
				int indexProp = 0;
				auto& bestProp = p[indexProp];
				while (std::find(drop.path.begin(), drop.path.end(), graph[drop.actualVertex].neighbors[bestProp.first].index) != drop.path.end())
				{
					++indexProp;
					if (indexProp == p.size())
					{
						drop.reset(0);
						drop.path.push_back(0);
						break;
					}
					bestProp = p[indexProp];
				}
				//graph[drop.actualVertex].high -= bestProp.second;
				drop.gradients.push_back(bestProp.second);
				//std::cout << "chyba tu\n";
				drop.actualDIstance += graph[drop.actualVertex].neighbors[bestProp.first].distance;
				drop.actualVertex = graph[drop.actualVertex].neighbors[bestProp.first].index;
				drop.path.push_back(drop.actualVertex);
				//std::cout <<"Actual position of drop " <<drop.actualVertex << "\n";
				//std::cin.get();
			}
		}
		erossion();
		addPrecipitate();
		checkAllPAthsAndTakeBest();
	}

	void erossion()
	{
		for (auto& drop : drops)
		{
			for (int i = 0; i < drop.gradients.size(); ++i)
			{
				graph[drop.path[i]].high -= drop.gradients[i];
			}
		}
	}

	double propability(int i,int j)
	{
		//std::cout << "Tu sie sypie? "<< graph[i].neighbors[j].distance<<"\n";
		double prop = ((graph[i].high - graph[graph[i].neighbors[j].index].high) / graph[i].neighbors[j].distance);
		//prop = prop < 0 ? 0 : prop;
		//std::cout << "propability of " << i << " " << graph[i].neighbors[j].index << " = " << prop<<"\n";
		return (graph[i].high - graph[graph[i].neighbors[j].index].high) / graph[i].neighbors[j].distance;
	}

	void addPrecipitate()
	{
		for (int i = 0; i < graphSize - 1; ++i)
		{
			graph[i].high += graph[i].high < 255 ? 1 : 0;
			graph[i].high = graph[i].high > 255 ? 255 : graph[i].high;
		}
	}
	
	void checkAllPAthsAndTakeBest()
	{
		
		bestPath.clear();
		end = true;
		auto it = std::min_element(std::begin(drops), std::end(drops), [](Drop& d1, Drop& d2) { return d1.actualDIstance < d2.actualDIstance; });
		std::cout << "\n\nCHECK PATCH!!!!!!!\n\n";
		std::cout << it->path.size() << "\n";
		for (int i = 0; i<it->path.size(); ++i)
			bestPath.push_back(it->path[i]);
		std::cout << "\n\n2CHECK PATCH!!!!!!!\n\n";
		for (int i = 0; i < drops.size() - 1; ++i)
		{
			if (drops[i].path.size() != drops[i + 1].path.size())
			{
				std::cout << "\n\n3CHECK PATCH!!!!!!!\n\n";
				end = false;
				break;
			}
			if (!(drops[i] == drops[i + 1]))
			{
				std::cout << "\n\n4CHECK PATCH!!!!!!!\n\n";
				end = false;
				break;
			}
		}
	}

private:
	std::unique_ptr<Vertex[]> graph;
	std::valarray<Drop> drops;
	int graphSize;
	bool end;
	std::vector<int> bestPath;
	std::unique_ptr<DrawGraph<Vertex>> drawGraph;
	std::vector<Vertex> vertexs;
};
#endif // !RIVER_FORMATION_DYNAMICS_H
