#ifndef DIFFERENTIAL_EVOLUTION_H
#define DIFFERENTIAL_EVOLUTION_H

#include <glm\glm.hpp>
#include <functional>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <memory>
#include <iostream>
#include <thread>
#include "MyRandom.h"
#include "DrawingClass.h"

class DifferentialEvolution
{
	struct Individual
	{
		glm::vec2 position;
		double f;
	};
public:
	DifferentialEvolution(std::function<double(double, double)> fun, const glm::vec2& v1, const glm::vec2& v2, int S = 20, double cr = 0.5, double F = 0.5)
	{
		this->f = fun;
		this->v1 = v1;
		this->v2 = v2;
		this->cr = cr;
		this->F = F;
		this->individuals.resize(S);
		this->u.resize(S);
		rnd = std::uniform_real_distribution<double>(0.0, 1.0);
		rnd2 = std::uniform_int_distribution<int>(0, S - 1);
		rv = TwoDimensionUniformRandom(v1, v2);
		engine = std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		drawingClass.reset(new DrawingClass(f, v1, v2));
	}

	void run(int maxIterations = 50)
	{
		initializePopulation();
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
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			if (t < maxIterations)
			{
				mutation();
				recombination();
				selection();
				++t;
			}
			drawingClass->drawPoinstOnFunction(u);
		}
	}

private:
	void initializePopulation()
	{
		for (auto& in : individuals)
		{
			in.position = rv(engine);
			in.f = f(in.position.x, in.position.y);
		}
		for (int i = 0; i < individuals.size(); ++i)
			u[i] = individuals[i].position;
	}

	void mutation()
	{
		for (int i = 0; i < individuals.size(); ++i)
		{
			int r1 = rnd2(engine);
			while (r1==i)
				r1 = rnd2(engine);

			int r2 = rnd2(engine);
			while (r2==i || r2 == r1)
				r2 = rnd2(engine);

			int r3 = rnd2(engine);
			while (r3 == i || r3 == r2 || r3 == r1)
				r3 = rnd2(engine);

			individuals[i].position = u[r1] + glm::vec2(F)*(u[r2] - u[r3]);
		}
	}

	void recombination()
	{
		for (int i = 0; i < individuals.size(); ++i)
		{
			if (rnd(engine) > cr)
				individuals[i].position.x = u[i].x;
			if (rnd(engine) > cr)
				individuals[i].position.y = u[i].y;
			individuals[i].f = f(individuals[i].position.x, individuals[i].position.y);
		}
	}

	void selection()
	{
		for (int i = 0; i < individuals.size(); ++i)
		{
			if (f(u[i].x, u[i].y)>individuals[i].f)
				u[i] = individuals[i].position;
			correctPosition(u[i]);
		}
	}

	void correctPosition(glm::vec2& uv)
	{
		glm::vec2 pos = rv(engine);
		if (uv.x<v1.x || uv.x>v2.x || uv.y<v1.y || uv.y >v2.y)
			uv = pos;
	}

private:
	std::valarray<Individual> individuals;
	std::valarray<glm::vec2> u;
	glm::vec2 v1, v2;
	double cr, F;
	std::function<double(double, double)> f;
	std::unique_ptr<DrawingClass> drawingClass;
	TwoDimensionUniformRandom rv;
	std::uniform_real_distribution<double> rnd;
	std::default_random_engine engine;
	std::uniform_int_distribution<int> rnd2;
};

#endif //!DIFFERENTIAL_EVOLUTION_H
