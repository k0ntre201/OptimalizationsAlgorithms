#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <glm/glm.hpp>
#include <algorithm>

struct Pair
{
	Pair(int i, double d) :index(i), distance(d){}
	int index;
	double distance;
};

struct Third
{
	Third(int i, double d, double s) :index(i), distance(d), soil(s) {}
	int index;
	double distance,soil;
};

struct Vertex
{
	int index;
	glm::vec2 position;
	double high;
	std::vector<Pair> neighbors;
};

struct VertexIWD
{
	int index;
	glm::vec2 position;
	double high;
	std::vector<Third> neighbors;
};

struct Drop
{
	int actualVertex;
	std::vector<int> path;
	std::vector<double> gradients;
	double actualDIstance;
	void reset(int av)
	{
		actualDIstance = 0.0;
		actualVertex = av;
		path.clear();
		gradients.clear();
	}
	bool operator==(const Drop& drop)
	{
		if (this->path.size() != drop.path.size())
			return false;
		else
		{
			if (!std::equal(std::begin(path), std::end(path), std::begin(drop.path), std::end(drop.path)))
				return false;
			/*for (int i = 0; i < this->path.size(); ++i)
			{
				if (this->path[i] != drop[i])
				{
					return false;
				}
			}*/
		}
		return true;
	}
};

struct IWD
{
	int actualVertex,prevoriusVertex;
	std::vector<int> path;
	//std::vector<double> soils;
	std::vector<std::pair<std::pair<int, int>, double>> soils;
	double actualDIstance,soilIWD,velocity;
	bool doCycle;
	void reset(int av,double initVel)
	{
		doCycle = false;
		velocity = initVel;
		actualDIstance = soilIWD = 0.0;
		actualVertex = av;
		path.clear();
		path.push_back(av);
		soils.clear();
	}
};

#endif //!VERTEX_H