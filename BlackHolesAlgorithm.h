#ifndef BLACK_HOLES_ALGORITHM_H
#define BLACK_HOLES_ALGORITHM_H

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

class BlackHolesAlgorithm
{
	struct BlackHole
	{
		glm::vec2 position;
		double f;
	};
public:
	BlackHolesAlgorithm(std::function<double(double, double)> fun, const glm::vec2& v1, const glm::vec2& v2, int N = 20, double F = 0.3)
	{
		this->f = fun;
		this->v1 = v1;
		this->v2 = v2;
		u.resize(N);
		blackHoles.resize(N);
		rnd = std::uniform_real_distribution<double>(0.0, 1.0);
		rnd2 = std::uniform_int_distribution<int>(0, N - 1);
		engine = std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		rv = TwoDimensionUniformRandom(v1, v2);
		drawingClass.reset(new DrawingClass(f, v1, v2));
	}

	void run(int maxIterations = 50)
	{
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
			std::cout << bestGlobal.position.x << " " << bestGlobal.position.y << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			if (t < maxIterations)
			{
				moveToNextPosition();
				fuzzyRadiation();
				evaluateFunction();
				bestLocal = getBest();
				checkBest();
				++t;
			}
			drawingClass->drawPoinstOnFunction(u);
		}

	}

private:
	void initialize()
	{
		for (int i = 0; i < blackHoles.size(); ++i)
		{
			u[i] = blackHoles[i].position = rv(engine);
			blackHoles[i].f = f(blackHoles[i].position.x, blackHoles[i].position.y);
		}
		bestGlobal = getBest();
		bestLocal = blackHoles[rnd2(engine)];
	}

	void moveToNextPosition()
	{
		for (auto& bh : blackHoles)
		{
			bh.position = bh.position + glm::vec2(rnd(engine), rnd(engine))*(bestLocal.position - bh.position) + glm::vec2(rnd(engine), rnd(engine))*(bestGlobal.position - bh.position);
		}
		for (int i = 0; i < blackHoles.size(); ++i)
			boundsLimit(blackHoles[i]);
	}

	void fuzzyRadiation()
	{
		for (int i = 0; i < blackHoles.size(); ++i)
		{
			int r1 = rnd2(engine);
			while (r1 == i)
				r1 = rnd2(engine);

			int r2 = rnd2(engine);
			while (r2 == i || r2 == r1)
				r2 = rnd2(engine);
			BlackHole bh;
			bh.position = blackHoles[i].position + glm::vec2(F)*(blackHoles[r1].position - blackHoles[r2].position);
			boundsLimit(bh);
			if(f(bh.position.x,bh.position.y)<blackHoles[i].f)
				 blackHoles[i] = bh;
			u[i] = blackHoles[i].position;
		}
	}

	void boundsLimit(BlackHole& bh)
	{
		glm::vec2 temp = rv(engine);
		if (bh.position.x<v1.x || bh.position.x>v2.x || bh.position.y<v1.y || bh.position.y>v2.y)
			bh.position = temp;
	}

	void evaluateFunction()
	{
		for (auto& bh : blackHoles)
			bh.f = f(bh.position.x, bh.position.y);
	}

	void checkBest()
	{
		if (bestLocal.f < bestGlobal.f)
			bestGlobal = bestLocal;
	}

	BlackHole getBest()
	{
		return *std::min_element(std::begin(blackHoles), std::end(blackHoles), [](BlackHole& bh1, BlackHole& bh2) { return bh1.f < bh2.f; });
	}
private:
	std::valarray<BlackHole> blackHoles;
	BlackHole bestGlobal, bestLocal;
	std::valarray<glm::vec2> u;
	glm::vec2 v1, v2;
	std::function<double(double, double)> f;
	std::unique_ptr<DrawingClass> drawingClass;
	TwoDimensionUniformRandom rv;
	std::uniform_real_distribution<double> rnd;
	std::default_random_engine engine;
	std::uniform_int_distribution<int> rnd2;
	double F;
};


#endif // !BLACK_HOLES_ALGORITHM_H
