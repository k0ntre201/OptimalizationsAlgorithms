#ifndef OA_H
#define OA_H

#include <random>
#include <chrono>
#include <glm\glm.hpp>
#include "MyRandom.h"
#include <valarray>

template<typename F>
void simulatedAnneaning(double& x,double& y, double& fx, F func)
{
	std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	double T0 = 8000;
	double Tf = 1e-4;
	double alpha = 0.999;
	std::uniform_real_distribution<double> dist1(-5, 5);
	std::uniform_real_distribution<double> dist2(-1.0, 1.0);
	std::uniform_real_distribution<double> dist3(0.0, 1.0);
	x = dist1(engine);
	y = dist1(engine);

	fx = func(x, y);
	while (T0>Tf)
	{
		double x2 = x + dist2(engine);
		double y2 = y + dist2(engine);

		double nextFx = func(x2, y2);
		double delta = nextFx - fx;

		if (delta < 0)
		{

			x = x2;
			y = y2;

			fx = nextFx;
		}
		else
		{
			double r = dist3(engine);
			if (r < std::exp(-delta / T0))
			{
				x = x2;
				y = y2;
				fx = nextFx;
			}
		}
		T0 *= alpha;
	}
}

template<typename F>
void simulatedAnneaning(glm::vec2& v, double& fxy, const glm::vec2& x1, const glm::vec2& x2, F f, double T0 = 800.0, double Tf = 10e-6, double alpha = 0.999)
{
	std::mt19937 engine(std::chrono::system_clock::now().time_since_epoch().count());
	TwoDimensionUniformRandom dist(x1, x2);
	TwoDimensionUniformRandom dist2(glm::vec2(-1, -1), glm::vec2(1, 1));
	std::uniform_real_distribution<double> dist3(0.0, 1.0);
	v = dist(engine);
	fxy = f(v);
	int i = 0;
	while (T0 > Tf)
	{
		glm::vec2 nV = v + dist(engine);
		double nextFx = f(nV);
		double delta = nextFx - fxy;
		if (delta < 0)
		{
			v = nV;
			fxy = nextFx;
		}
		else
		{
			double r = dist3(engine);
			if (r < std::exp(-delta / T0))
			{
				v = nV;
				fxy = nextFx;
			}
		}
		T0 *= alpha;
		++i;
	}
}

template <typename F>
void fireflyAlgorithm(glm::vec2& x, double& fxy, F f)
{

}

struct Particle
{
	glm::vec2 pos;
	glm::vec2 vel;
	double f;
};


template <typename F>
void particleSwarmOptimalization(glm::vec2& g, double& fxy, const glm::vec2& x1, const glm::vec2& x2, F f, std::size_t N = 100, double w = 0.7,double alpha = 2.0, double beta = 2.0)
{
	std::default_random_engine engine(std::chrono::system_clock::now().time_since_epoch().count());
	TwoDimensionUniformRandom dist1(x1, x2);
	TwoDimensionUniformRandom dist2(glm::vec2(0.0), glm::vec2(1.0));
	g = dist1(engine);
	std::valarray<Particle> x(N);
	std::valarray<glm::vec2> p(N);
	for (int i = 0; i < N; ++i)
	{
		p[i] = x[i].pos = dist1(engine);
		x[i].vel = glm::vec2(0.0);
		if (f(p[i]) < f(g))
			g = p[i];
	}

	for (int t = 0; t < 25; t++)
	{
		for (int i = 0; i < N; ++i)
		{
			glm::vec2 rp = dist2(engine);
			glm::vec2 rg = dist2(engine);
			x[i].vel = (x[i].vel*glm::vec2(w) + rp*(p[i] - x[i].pos)*glm::vec2(alpha) + rg*(g - x[i].pos)*glm::vec2(beta));
			x[i].pos += x[i].vel;
			if (f(x[i].pos) < f(p[i]))
			{
				p[i] = x[i].pos;
				if (f(p[i]) < f(g))
				{
					g = p[i];
					fxy = f(g);
				}
			}
		}
	}
}

#endif // !OA_H
