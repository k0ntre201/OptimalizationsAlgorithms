#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>
#include <glm\glm.hpp>

double rossenbrock(double x, double y)
{
	return ((1 - x)*(1 - x) + 100*( y - x*x)*(y - x*x));
}

double rastigrin(double x, double y)
{
	return ((x*x + y*y) + 25 * (std::sin(x)*std::sin(x) + std::cos(y)*std::cos(y)));
}

double rossenbrockv(glm::vec2 v)
{
	return rossenbrock(v.x,v.y);
}

double rastigrinv(glm::vec2 v)
{
	return rastigrin(v.x,v.y);
}

#endif // !FUNCTIONS_H
