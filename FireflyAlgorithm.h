#ifndef FIREFLY_ALGORITHM_H
#define FIREFLY_ALGORITHM_H

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

class FireflyAlgorithm
{
	struct FireFly
	{
		glm::vec2 position;
		glm::vec2 oldPosition;
		double I;
	};
public:
	FireflyAlgorithm(std::function<double(double, double)> fun, glm::vec2& v1, glm::vec2& v2, std::size_t N = 20, double beta = 1.0, double alpha = 1.0, double delta = 0.97)
	{
		this->f = fun;
		this->v1 = v1;
		this->v2 = v2;
		this->beta = beta;
		this->alpha = alpha;
		this->delta = delta;
		this->gamma = glm::vec2(1.0) / glm::sqrt(glm::vec2(v2 - v1));
		drawingClass.reset(new DrawingClass(fun, v1, v2));
		fireFlyes.resize(N);
	}
	void run(int maxIterations = 50)
	{
		std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
		TwoDimensionUniformRandom dist1(v1, v2);
		TwoDimensionUniformRandom dist2(glm::vec2(-.5), glm::vec2(.5));
		glm::vec2 g = dist1(engine);
		double fxy = f(g.x,g.y);
		std::valarray<glm::vec2> v(fireFlyes.size());
		for (int i = 0; i < fireFlyes.size(); ++i)
		{
			v[i] = fireFlyes[i].position = dist1(engine);
			fireFlyes[i].I = f(fireFlyes[i].position.x, fireFlyes[i].position.y);
			if (fireFlyes[i].I < fxy)
			{
				fxy = fireFlyes[i].I;
				g = fireFlyes[i].position;
			}
		}

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
				for (int i = 0; i < fireFlyes.size(); ++i)
				{
					fireFlyes[i].oldPosition = fireFlyes[i].position;
				}
				for (int i = 0; i < fireFlyes.size(); ++i)
				{
					for (int j = 0; j < fireFlyes.size(); ++j)
					{
						if (i != j)
						{
							if (fireFlyes[i].I > fireFlyes[j].I)
							{
								double distance = glm::distance(fireFlyes[i].oldPosition, fireFlyes[j].oldPosition);
								glm::vec2 atractivnes = glm::exp(-gamma*glm::vec2(distance*distance));
								v[i] = fireFlyes[i].position = fireFlyes[i].oldPosition + atractivnes*(fireFlyes[j].oldPosition - fireFlyes[i].oldPosition) + glm::vec2(alpha)*dist2(engine);
								glm::vec2 temp = dist1(engine);
								if (v[i].x<v1.x || v[i].x>v2.x)
									v[i].x = fireFlyes[i].position.x = temp.x;
								if (v[i].y<v1.y || v[i].y>v2.y)
									v[i].y = fireFlyes[i].position.y = temp.y;
								fireFlyes[i].I = f(fireFlyes[i].position.x, fireFlyes[i].position.y);
								if (fireFlyes[i].I < fxy)
								{
									fxy = fireFlyes[i].I;
									g = fireFlyes[i].position;
								}
							}
						}
					}
				}
				alpha *= delta;
				++t;
			}
			drawingClass->drawPoinstOnFunction(v);
			std::cout << t << "\n";
			std::cout << g.x << " " << g.y << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			
		}

	}
private:
	std::valarray<FireFly> fireFlyes;
	double beta,delta,alpha;
	std::unique_ptr<DrawingClass> drawingClass;
	std::function<double(double, double)> f;
	glm::vec2 v1, v2, gamma;
};
#endif // !FIREFLY_ALGORITHM_H
