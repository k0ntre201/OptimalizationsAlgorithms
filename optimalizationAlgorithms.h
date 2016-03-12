#ifndef OA_H
#define OA_H

#include <random>
#include <chrono>

template<typename F>
void simulatedAnneaning(double& x,double& y, double& fx, F func)
{
	std::default_random_engine engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	double T0 = 8000;
	double Tf = 1e-4;
	double alpha = 0.999;
	std::uniform_real_distribution<double> dist1(-2, 2);
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
		T0 =T0* alpha;
	}
}

#endif // !OA_H
