#ifndef BAT_ALGORITHM_H
#define BAT_ALGORITHM_H

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
#include <numeric>

class BatAlgorithm
{
	struct Bat
	{
		glm::vec2 position, veliocity;
		double f, A, r;
		double fun;
	};
public:
	BatAlgorithm(std::function<double(double, double)> f, const glm::vec2& v1, const glm::vec2 v2, int N = 50, double alpha = 0.9, double gamma = 0.9, double fmin = 0, double fmax = 2)
	{
		this->f = f;
		this->v1 = v1;
		this->v2 = v2;
		this->alpha = alpha;
		this->gamma = gamma;
		this->N = N;
		this->fmin = fmin;
		this->fmax = fmax;
		glm::vec2 temp = (v2 - v1)*glm::vec2(0.1);
		this->fmax = temp.x;
		this->random = TwoDimensionUniformRandom(v1, v2);
		this->random2 = TwoDimensionUniformRandom(glm::vec2(-1), glm::vec2(1));
		this->beta = std::uniform_real_distribution<double>(0.0, 1.0);
		this->r = std::uniform_real_distribution<double>(0.0, 1);
		this->drawingClass.reset(new DrawingClass(f, v1, v2));
		this->bats.resize(N);
		this->v.resize(N);
		engine = std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	}
	void run(int maxIterations = 50)
	{
		initialize();
		SDL_Event e;
		bool quit = false;
		int t = 0;
		bool end;
		calculateFunction();
		bestSoluion = sortBatsAndReturnBest();
		while (!quit)
		{
			if (SDL_PollEvent(&e))
				if (e.type == SDL_QUIT)
					quit = true;



			std::cout << t << "\n";
			std::cout << bestSoluion.x << " " << bestSoluion.y << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			//std::cin.get();
			if (t < maxIterations)
			{
				calculateAverangeLaught();
				for (int i = 0; i < N; ++i)
				{
					actualizeFrequencyPositionVelocity(i,bestSoluion);
					bestLocalSolutions(i, t);
					calculateNewRandomSolution(i, t, bestSoluion);
					correctPosition(i);
				}
				calculateFunction();
				glm::vec2 temp = sortBatsAndReturnBest();
				//if (f(temp.x, temp.y) < f(bestSoluion.x, bestSoluion.y))
					//bestSoluion = temp;
				positionsToDraw();
				++t;
			}
			drawingClass->drawPoinstOnFunction(v);
			//std::cin.get();
		}
	}
private:
	void initialize()
	{
		for (int i = 0; i < bats.size(); ++i)
		{
			v[i] = bats[i].position = random(engine);
			bats[i].f = fmin + (fmax - fmin)*beta(engine);
			bats[i].A = 1;
			bats[i].r = 1;
			bats[i].veliocity = glm::vec2(0.0);
		}
	}

	void calculateFunction()
	{
		for (size_t i = 0; i < bats.size(); i++)
		{
			bats[i].fun = f(bats[i].position.x, bats[i].position.y);
		}
	}

	glm::vec2 sortBatsAndReturnBest()
	{
		std::sort(std::begin(bats), std::end(bats), [](Bat a, Bat b) { return (a.fun < b.fun); });
		return bats[0].position;
	}

	void actualizeFrequencyPositionVelocity(int i,const glm::vec2& besPosition)
	{
		bats[i].f = fmin + (fmax - fmin)*beta(engine);
		bats[i].veliocity = ((bestSoluion - bats[i].position)*glm::vec2(bats[i].f));
		//if (i == 0)
			//std::cout << "v " << bats[i].veliocity.x << " " << bats[i].veliocity.y << "\n";
		bats[i].position += bats[i].veliocity;
		correctPosition(i);
	}

	void calculateAverangeLaught()
	{
		averangeLaught = std::accumulate(std::begin(bats), std::end(bats), 0.0, []( double val, const Bat& b2) { return val + b2.A; })/N;
	}

	void bestLocalSolutions(int i,int t)
	{
		int localN = N / 4;
		if (r(engine) > bats[i].r)
		{
			bats[i%localN].position +=random2(engine)*glm::vec2(averangeLaught);
			correctPosition(i%localN);
		}
	}

	void calculateNewRandomSolution(int i,int t,const glm::vec2& best)
	{
		//glm::vec2 newSolution = random(engine);
		if (r(engine) < bats[i].A && bats[i].fun < f(best.x, best.y))
		{
			bestSoluion = bats[i].position;
			bats[i].A *= alpha;
			bats[i].r = (1 - std::exp(-gamma*t));
		}
	}

	void positionsToDraw()
	{
		for (int i = 0; i < bats.size(); ++i)
			v[i] = bats[i].position;
	}

	void correctPosition(int indx)
	{
		glm::vec2 newPos = random(engine);
		if (bats[indx].position.x<v1.x || bats[indx].position.x>v2.x)
			bats[indx].position.x = newPos.x;
		if (bats[indx].position.y<v1.y || bats[indx].position.y>v2.y)
			bats[indx].position.y = newPos.y;
	}

private:
	std::valarray<Bat> bats;
	std::valarray<glm::vec2> v;
	glm::vec2 v1, v2,bestSoluion;
	double fbest;
	double alpha, gamma, fmax, fmin;
	std::unique_ptr<DrawingClass> drawingClass;
	TwoDimensionUniformRandom random;
	TwoDimensionUniformRandom random2;
	std::uniform_real_distribution<double> beta,r;
	std::default_random_engine engine;
	std::function<double(double, double)> f;
	int N;
	double averangeLaught;
};
#endif //!BAT_ALGORITHM_H