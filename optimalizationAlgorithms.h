#ifndef OA_H
#define OA_H

#include <random>
#include <chrono>
#include <glm\glm.hpp>
#include "MyRandom.h"

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
	}
}

#endif // !OA_H
