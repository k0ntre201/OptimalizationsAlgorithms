#ifndef INTELLINENT_WATER_DROPS_H
#define INTELLINENT_WATER_DROPS_H

#include <memory>
#include <valarray>
#include "Vertex.h"
#include <random>
#include <chrono>
#include <numeric>
#include <string>
#include <fstream>
#include "DrawGraph.h"
#include <iostream>
#include <thread>

class IntelligentWaterDrops
{
public:
	IntelligentWaterDrops(
		const std::string& graphName,
		double av = 1.0,
		double bv = 0.01,
		double cv = 1.0,
		double as = 1.0,
		double bs = 0.01,
		double cs = 1.0,
		double pn = 0.9,
		double piwd = -0.9,
		double initSoil = 1.0,
		double initVel = 1.0
		)
	{
		this->av = av;
		this->bv = bv;
		this->cv = cv;
		this->as = as;
		this->bs = bs;
		this->cs = cs;
		this->pn = pn;
		this->piwd = piwd;
		this->initSoil = initSoil;
		this->initVel = initVel;
		FILE* file;
		fopen_s(&file, graphName.c_str(), "r");
		if (file != nullptr)
		{
			fscanf(file, "%d", &graphSize);
			graph.reset(new VertexIWD[graphSize]);
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
				graph[s].neighbors.push_back(Third(d, w,initSoil));
				graph[d].neighbors.push_back(Third(s, w,initVel));
			}
			for (int i = 0; i < graphSize; ++i)
				vertexs.push_back(graph[i]);
			drawGraph.reset(new DrawGraph<VertexIWD>(std::move(vertexs)));
			int Niwd = vertexs.size() / 4;
			IWDs.resize(Niwd);
		}
		else
		{
			std::cout << "Cannot open file " << graphName << "\n";
		}
	}

	void run(int maxIteratiins = 10)
	{
		end = false;
		int t = 0;
		
		while (!end)
		{
			if (t < maxIteratiins)
			{
				std::cout << "start nr " << t << "\n";

				initializeDynamicsParametrs();
				while (!allIwdSIsEnd())
				{
					int k = 0;
					for (auto& iwd : IWDs)
					{
						iwd.doCycle = false;
						if (iwd.actualVertex != graph[graphSize - 1].index)
						{
							int i = calculatePropability(iwd);;
							if(i>-1)
								modeDropsandUpdateVelocity(iwd, i);
						}
						//std::cout << "Iwd nr " << k << " " << iwd.actualVertex << "\n";
						//std::cin.get();
						k++;
					}
					
				}
				for (auto& iwd : IWDs)
				{
					if (iwd.actualVertex != graph[graphSize - 1].index && iwd.path.size()>1 && !iwd.doCycle)
						updateSoils(iwd);
					//updateVistedNodes(iwd);
				}
				findIterationBestPath();
				++t;
				//std::cin.get();
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}
			else
				end = true;
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
	void initializeDynamicsParametrs()
	{
		for (auto& iwd : IWDs)
			iwd.reset(0, initVel);
		visitedNodes.clear();
	}

	int calculatePropability(IWD& iwd)
	{
		std::vector<std::pair<int, double>> p;
		auto soilMin = std::min_element(std::begin(graph[iwd.actualVertex].neighbors), std::end(graph[iwd.actualVertex].neighbors), [](Third& v1, Third& v2) {return v1.soil < v2.soil; });
		for (int i = 0; i < graph[iwd.actualVertex].neighbors.size(); ++i)
		{
			int neghberhoodIndex = graph[iwd.actualVertex].neighbors[i].index;
			auto it = std::find(std::begin(iwd.path), std::end(iwd.path), neghberhoodIndex);
			auto it2 = std::find(std::begin(visitedNodes), std::end(visitedNodes), neghberhoodIndex);
			if (it != iwd.path.end()) //|| it2!=visitedNodes.end())
				continue;
			double prop = (soilMin->soil >= 0) ? 1.0 / (0.01 + graph[iwd.actualVertex].neighbors[i].soil) : 1.0 / (0.01 + graph[iwd.actualVertex].neighbors[i].soil - soilMin->soil);
			p.push_back(std::make_pair(i, prop));
		}
		
		if (!p.empty())
		{
			std::shuffle(std::begin(p), std::end(p), std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
			std::sort(std::begin(p), std::end(p), [](std::pair<int, double>& p1, std::pair<int, double>& p2) {return p1.second > p2.second; });
			return p[0].first;
		}
		iwd.reset(0, initVel);
		iwd.doCycle = true;
		return -1;
	}

	void modeDropsandUpdateVelocity(IWD& iwd, int i)
	{
		iwd.velocity += (av) / (bv + cv* graph[iwd.actualVertex].neighbors[i].soil* graph[iwd.actualVertex].neighbors[i].soil);
		iwd.actualDIstance += graph[iwd.actualVertex].neighbors[i].distance;
		double time = graph[iwd.actualVertex].neighbors[i].distance / iwd.velocity;
		double deltaSoil = (as) / (bs + cs*time*time);
		iwd.soils.push_back(std::make_pair(std::make_pair(iwd.actualVertex, i), deltaSoil));
		iwd.soilIWD += deltaSoil;
		iwd.prevoriusVertex = iwd.actualVertex;
		iwd.actualVertex = graph[iwd.actualVertex].neighbors[i].index;
		iwd.path.push_back(iwd.actualVertex);
	}

	void updateSoils(IWD& iwd)
	{
		
		/*auto it1 = std::find_if(std::begin(graph[iwd.prevoriusVertex].neighbors), std::end(graph[iwd.prevoriusVertex].neighbors), [=](Third& t2) {return t2.index == iwd.actualVertex; });
		double time = it1->distance / iwd.velocity;
		double deltaSoil = (as) / (bs + cs*time*time);
		iwd.soilIWD += deltaSoil;
		it1->soil = (1 - pn)*it1->soil - pn*deltaSoil;
		auto it2 = std::find_if(std::begin(graph[iwd.actualVertex].neighbors), std::end(graph[iwd.actualVertex].neighbors), [=](Third& t2) {return t2.index == iwd.prevoriusVertex; });
		it2->soil = (1 - pn)*it2->soil - pn*deltaSoil;*/

		for (int i = 0; i < iwd.soils.size(); ++i)
		{
			const auto& w = iwd.soils[i];
			graph[w.first.first].neighbors[w.first.second].soil = (1 - pn)*graph[w.first.first].neighbors[w.first.second].soil - pn*w.second;
			int acvert = graph[w.first.first].neighbors[w.first.second].index;
			auto it = std::find_if(std::begin(graph[acvert].neighbors), std::end(graph[acvert].neighbors), [=](Third& t2) { return t2.index == w.first.first; });
			it->soil = (1 - pn)*it->soil - pn*w.second;
		}
	}

	void updateVistedNodes(IWD& iwd)
	{
		auto it = std::find(std::begin(visitedNodes), std::end(visitedNodes), iwd.prevoriusVertex);
		if (it == visitedNodes.end())
			visitedNodes.push_back(iwd.prevoriusVertex);
	}

	void findIterationBestPath()
	{
		auto itBestPath = std::min_element(std::begin(IWDs), std::end(IWDs), [](IWD& i1, IWD& i2) { return i1.actualDIstance < i2.actualDIstance; });
		/*for (int i = 0; i < itBestPath->path.size()-1; ++i)
		{
			auto n1 = std::find_if(graph[itBestPath->path[i]].neighbors.begin(), graph[itBestPath->path[i]].neighbors.end(), [=](Third& t) { return t.index == itBestPath->path[i + 1]; });
			n1->soil = (1-piwd)*n1->soil + piwd / (IWDs.size() - 1)*itBestPath->soilIWD;
			auto n2 = std::find_if(graph[itBestPath->path[i+1]].neighbors.begin(), graph[itBestPath->path[i+1]].neighbors.end(), [=](Third& t) { return t.index == itBestPath->path[i]; });
			n2->soil = (1 - piwd)*n2->soil + piwd / (IWDs.size() - 1)*itBestPath->soilIWD;

			//graph[itBestPath->path[i]].neighbors[itBestPath->path[i + 1]].soil = (1 - piwd)*graph[itBestPath->path[i]].neighbors[itBestPath->path[i + 1]].soil + piwd / (IWDs.size() - 1)*itBestPath->soilIWD;
			//graph[itBestPath->path[i + 1]].neighbors[itBestPath->path[i]].soil = (1 - piwd)*graph[itBestPath->path[i + 1]].neighbors[itBestPath->path[i]].soil + piwd / (IWDs.size() - 1)*itBestPath->soilIWD;

		}*/
		if (bestPath.empty())
		{
			actualDistance = itBestPath->actualDIstance;
			for (int i = 0; i < itBestPath->path.size(); ++i)
			{
				bestPath.push_back(itBestPath->path[i]);
				//std::cout << itBestPath->path[i] << " ";
			}
		}
		else
		{
			if (actualDistance > itBestPath->actualDIstance)
			{
				bestPath.clear();
				actualDistance = itBestPath->actualDIstance;
				for (int i = 0; i < itBestPath->path.size(); ++i)
					bestPath.push_back(itBestPath->path[i]);
			}
		}
		for (int i = 0; i < itBestPath->path.size(); ++i)
		{
			std::cout << itBestPath->path[i] << " ";
		}
		std::cout << "\n";
		//drawGraph->draw(itBestPath->path, 0, 255, 0);
	}

	bool allIwdSIsEnd()
	{
		for (int i = 0; i < IWDs.size(); ++i)
		{
			if (IWDs[i].actualVertex != graph[graphSize - 1].index)
				return false;
		}
		return true;
	}

private:
	std::unique_ptr<VertexIWD[]> graph;
	int graphSize;
	bool end;
	std::vector<int> bestPath;
	double actualDistance;
	std::unique_ptr<DrawGraph<VertexIWD>> drawGraph;
	std::vector<VertexIWD> vertexs;
	std::valarray<IWD> IWDs;
	std::vector<int> visitedNodes;
	double av, bv, cv, as, bs, cs, pn, piwd, initSoil, initVel;

};
#endif // !INTELLINENT_WATER_DROPS_H

