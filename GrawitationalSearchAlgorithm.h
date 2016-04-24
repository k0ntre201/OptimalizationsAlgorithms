#ifndef GRAWITATIONAL_SEARCH_ALGORITHM_H
#define GRAWITATIONAL_SEARCH_ALGORITHM_H

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

class GrawitationalSearchAlgorithm
{
	struct Agent
	{
		glm::vec2 position, oldPosition, veliocity, accelerate;
		double m,f,M;
		bool operator<(const Agent& agent)
		{
			return this->f < agent.f;
		}
	};
public:
	GrawitationalSearchAlgorithm(std::function<double(double, double)> fun, const glm::vec2& v1, const glm::vec2& v2, int N = 20, double G0 = 1000,double alpha = 20)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->f = fun;
		this->G = G0;
		this->alpha = alpha;
		this->kBestSize = N;
		agents.resize(N);
		v.resize(N);
		random = TwoDimensionUniformRandom(v1, v2);
		random2 = TwoDimensionUniformRandom(glm::vec2(0.0), glm::vec2(1.0));
		engine = std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		drawingClass.reset(new DrawingClass(f, v1, v2));
		epsilon = 10e-6;
	}

	void run(int maxIterations = 50)
	{
		hLinearDecreasse = kBestSize / maxIterations;
		initialize();

		SDL_Event e;
		bool quit = false;
		int t = 0;
		bool end;
		while (!quit)
		{
			if (SDL_PollEvent(&e))
				if (e.type == SDL_QUIT)
					quit = true;


			
			std::cout << t << "\n";
			std::cout << agents[0].position.x << " " << agents[0].position.x << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			//std::cin.get();
			if (t < maxIterations && kBestSize>=1)
			{
				positionToOldPOsition();
				double g = evaluateCostant(t, maxIterations);
				evaluateFitness();
				calculateMasses();
				for (int i = 0; i < agents.size(); ++i)
				{
					computeAccelerate(i, g);
					updatePositionAndVelocity(i);
					correctPosition(i);
				}
				linearDecreasseOfKBest();
				++t;

			}
			drawingClass->drawPoinstOnFunction(v);
		}
	}
private:
	void initialize()
	{
		for (int i = 0; i < agents.size(); ++i)
		{
			agents[i].veliocity = glm::vec2(0);
			agents[i].accelerate = glm::vec2(0);
			agents[i].position = random(engine);
		}
	}
	double evaluateCostant(double t,double tmax)
	{
		return G*std::exp(-alpha*t / tmax);
	}

	void evaluateFitness()
	{
		for (int i = 0; i < agents.size(); ++i)
		{
			agents[i].f = f(agents[i].position.x, agents[i].position.y);
		}
		std::sort(&agents[0], &agents[0] + agents.size());
		best = agents[0].f;
		worst = agents[agents.size() - 1].f;
		//std::cout << worst << "\n";
		//std::cin.get();
	}

	void calculateAgensMass(int i)
	{
		agents[i].m = (agents[i].f - worst) / (best - worst);
	}

	void calculateInteriaMasses()
	{
		interiaMass = 0;
		for (int i = 0; i < agents.size(); ++i)
		{
			interiaMass += agents[i].m;
		}
	}

	void calculateAgentInteriaMass(int i)
	{
		agents[i].M = agents[i].m / interiaMass;
	}

	void calculateMasses()
	{
		for (int i = 0; i < agents.size(); ++i)
		{
			calculateAgensMass(i);
		}
		
		calculateInteriaMasses();
		for (int i = 0; i < agents.size(); ++i)
		{
			calculateAgentInteriaMass(i);
		}
	}

	glm::vec2 caclulateForce(int i,double g)
	{
		glm::vec2 F = glm::vec2(0);
		for (int j = 0; j <kBestSize; ++j)
		{
			if (j != i)
			{
				double r = glm::distance(agents[i].oldPosition, agents[j].oldPosition);
				F += random2(engine)*glm::vec2(g*(agents[i].M*agents[j].M / (r + 0.1)))*(agents[j].oldPosition - agents[i].oldPosition);
			}
		}
		return F;
	}

	void computeAccelerate(int i, double g)
	{
			
			glm::vec2 force = caclulateForce(i, g);
			if (agents[i].M > epsilon)
				agents[i].accelerate = force / glm::vec2(agents[i].M);
			else
				agents[i].accelerate = force / glm::vec2(0.1);
	}

	void updatePositionAndVelocity(int i)
	{
		//std::cout << "Agent " << i << " velo " << agents[i].veliocity.x << " " << agents[i].veliocity.y << "\n";

		glm::vec2 ra = random2(engine);
		//std::cout << "ra " << ra.x << " " << ra.y << "\n";
		agents[i].veliocity = ra*agents[i].veliocity + agents[i].accelerate;
		agents[i].position = agents[i].oldPosition + agents[i].veliocity;


		//std::cout << "Agent " << i << " veln " << agents[i].veliocity.x << " " << agents[i].veliocity.y << "\n";
		//std::cout << "Agent " << i << " accelerate " << agents[i].accelerate.x << " " << agents[i].accelerate.y << "\n";
	}

	void linearDecreasseOfKBest()
	{
		kBestSize -= hLinearDecreasse;
	}

	void positionToOldPOsition()
	{
		for (int i = 0; i < agents.size(); ++i)
			v[i] = agents[i].oldPosition = agents[i].position;
	}

	void correctPosition(int indx)
	{
		glm::vec2 newPos = random(engine);
		if (agents[indx].position.x<v1.x || agents[indx].position.x>v2.x)
			agents[indx].position.x = newPos.x;
		if (agents[indx].position.y<v1.y || agents[indx].position.y>v2.y)
			agents[indx].position.y = newPos.y;
	}

private:
	std::valarray<Agent> agents;
	std::valarray<glm::vec2> v;
	std::unique_ptr<DrawingClass> drawingClass;
	TwoDimensionUniformRandom random;
	TwoDimensionUniformRandom random2;
	std::default_random_engine engine;
	double kBestSize,hLinearDecreasse;
	glm::vec2 v1, v2;
	double epsilon;
	double G,alpha;
	std::function<double(double, double)> f;
	double best, worst, interiaMass;
};
#endif // !GRAWITATIONAL_SEARCH_ALGORITHM_H
