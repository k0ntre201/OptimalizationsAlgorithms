#ifndef PSO_H
#define PSO_H

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

class Pso
{
	struct Particle
	{
		glm::vec2 pos;
		glm::vec2 vel;
		double f;
	};
public:
	Pso(std::function<double(double, double)> fun, const glm::vec2& v1, const glm::vec2& v2, std::size_t N = 100, double w = 0.7, double alpha = 2.0, double beta = 2.0)
	{
		this->f = fun;
		this->v1 = v1;
		this->v2 = v2;
		this->N = N;
		this->alpha = alpha;
		this->beta = beta;
		this->w = w;
		drawingClass.reset(new DrawingClass(fun, v1, v2));
		v.resize(N);
		p.resize(N);
	}


public:
	void run(int maxIterations = 25)
	{
		std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
		TwoDimensionUniformRandom dist1(v1, v2);
		TwoDimensionUniformRandom dist2(glm::vec2(0.0), glm::vec2(1.0));
		g = dist1(engine);
		double fxy = 0;
		for (int i = 0; i < N; ++i)
		{
			p[i] = v[i].pos = dist1(engine);
			v[i].vel = glm::vec2(0.0);
			if (f(p[i].x,p[i].y) < f(g.x,g.y))
				g = p[i];
		}
		fxy = f(g.x, g.y);
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
				for (int i = 0; i < N; ++i)
				{
					glm::vec2 rp = dist2(engine);
					glm::vec2 rg = dist2(engine);
					v[i].vel = (v[i].vel*glm::vec2(w) + rp*(p[i] - v[i].pos)*glm::vec2(alpha) + rg*(g - v[i].pos)*glm::vec2(beta));
					v[i].pos += v[i].vel;

					glm::vec2 temp = dist1(engine);
					if (v[i].pos.x<v1.x || v[i].pos.x>v2.x)
						v[i].pos.x = temp.x;
					if (v[i].pos.y<v1.y || v[i].pos.y>v2.y)
						v[i].pos.y = temp.y;
					
					if (f(v[i].pos.x, v[i].pos.y) < f(p[i].x, p[i].y))
					{
						p[i] = v[i].pos;
						if (f(p[i].x, p[i].y) < f(g.x, g.y))
						{
							g = p[i];
							fxy = f(g.x, g.y);
						}
					}
				}
				t++;
			}
			drawingClass->drawPoinstOnFunction(p);
			std::cout << t << "\n";
			std::cout << g.x << " " << g.y << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
private:
	std::unique_ptr<DrawingClass> drawingClass;
	glm::vec2 v1, v2;
	std::valarray<Particle> v;
	std::valarray<glm::vec2> p;
	glm::vec2 g;
	std::function<double(double, double)> f;
	std::size_t N;
	double w, alpha, beta;
};
#endif // !PSO_H
