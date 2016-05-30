#ifndef GLOWWARM_SWARM_ALGORITHM_H
#define GLOWWARM_SWARM_ALGORITHM_H

#include <iostream>
#include <memory>
#include "DrawingClass.h"
#include "functions.h"
#include <functional>
#include "MyRandom.h"
#include <chrono>
#include <random>
#include <thread>
#include <valarray>
#include <vector>
#include <algorithm>

class GlowwarmSwarmAlgorithm
{
	struct Glowworm
	{
		glm::vec2 position,oldPosition;
		double l;
		std::vector<int> N;
		double rd;
	};
public:
	GlowwarmSwarmAlgorithm(std::function<double(double, double)> fun, const glm::vec2& v1, const glm::vec2& v2, int numOfGlowworms = 20,double rs = 5, double pico = 0.4, double beta = 0.08, double gamma = 0.6, double s = 0.03, double nt = 5, double l0 = 5)
	{
		this->f = fun;
		this->beta = beta;
		this->gamma = gamma;
		this->l0 = l0;
		this->nt = nt;
		this->s = s;
		this->pico = pico;
		this->v1 = v1;
		this->v2 = v2;
		drawingClass.reset(new DrawingClass(fun, v1, v2));
		glowworms.resize(numOfGlowworms);
		v.resize(numOfGlowworms);
	}

	void run(int maxIterations = 50)
	{
		init();
		SDL_Event e;
		bool quit = false;
		int t = 0;
		bool end;
		while (!quit)
		{
			if (SDL_PollEvent(&e))
				if (e.type == SDL_QUIT)
					quit = true;
			if (t < maxIterations)
			{
				for (int i = 0; i < glowworms.size(); ++i)
				{
					glowworms[i].N.clear();
					updateLucifein(i);
					glowworms[i].oldPosition = glowworms[i].position;
				}
				for (int i = 0; i < glowworms.size(); ++i)
				{
					for (int j = 0; j < glowworms.size(); ++j)
					{
						addNeighbours(i, j);
					}
					for (int j = 0; j < glowworms[i].N.size(); ++j)
					{
						int k = glowworms[i].N[j];
						double p = calculatePropability(i,k);
						updatePosition(i, k);
					}
					updateDistanceRadius(i);
				}
				++t;
			}
			drawingClass->drawPoinstOnFunction(v);
			//std::cin.get();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			std::cout << t << "\n";
		}
	}

private:
	std::valarray<Glowworm> glowworms;
	std::valarray<glm::vec2> v;
	double pico, beta, gamma, s, nt, l0, rs;
	std::unique_ptr<DrawingClass> drawingClass;
	std::function<double(double, double)> f;
	glm::vec2 v1, v2;

private:
	void addNeighbours(int i, int j)
	{
		if (glm::distance(glowworms[i].oldPosition, glowworms[j].oldPosition) < glowworms[i].rd && glowworms[i].l > glowworms[j].l)
			glowworms[i].N.push_back(j);
	}
	double calculatePropability(int i, int j)
	{
		double temp = 0;
		for (int k = 0; k < glowworms[i].N.size(); ++k)
			temp += glowworms[glowworms[i].N[k]].l - glowworms[i].l;
		return (glowworms[j].l - glowworms[i].l) / temp;
	}
	void updatePosition(int i,int j)
	{
		glm::vec2 temp = glowworms[j].oldPosition - glowworms[i].oldPosition;
		v[i] = glowworms[i].position = glowworms[i].oldPosition + glm::vec2(s)*(temp) / glm::vec2(glm::length(temp));
	}

	void updateLucifein(int i)
	{
		glowworms[i].l = (1 - pico)*glowworms[i].l + gamma*f(glowworms[i].position.x, glowworms[i].position.y);
	}

	void updateDistanceRadius(int i)
	{
		glowworms[i].rd = std::min(rs, std::max(0.0, glowworms[i].rd + beta*(nt - glowworms[i].N.size())));
	}

	void correctPosiotion(int i)
	{
	}

	void init()
	{
		glm::vec2 v = v2 - v1;
		rs = std::min(v.x, v.y) / 2;
		double r0 = rs / 2;
		TwoDimensionUniformRandom dist(v1, v2);
		std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
		for (int i = 0; i < glowworms.size(); ++i)
		{
			glowworms[i].position = dist(engine);
			glowworms[i].rd = r0;
			glowworms[i].l = (1 - pico)*l0 + gamma*f(glowworms[i].position.x, glowworms[i].position.y);
			glowworms[i].N.clear();
		}
	}
};
#endif // !GLOWWARM_SWARM_ALGORITHM_H
