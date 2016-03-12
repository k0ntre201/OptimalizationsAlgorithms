#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>

double rossenbrock(double x, double y)
{
	return ((1-x)*(1 - x) + 100*(y-x*x)*(y - x*x));
}

double rastigrin(double x, double y)
{
	return ((x*x + y*y) + 25 * (std::sin(x)*std::sin(x) + std::cos(y)*std::cos(y)));
}

#endif // !FUNCTIONS_H
