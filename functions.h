#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>
#include <glm\glm.hpp>
#include <functional>
#include <tuple>

enum Functions
{
	ROSENBROCK,
	RASTIGIN,
	PARABOLOID,
	FOUR_PICK_INVERSE,
	FOUR_PEAKS_INVERSE,
	BAEALE,
	GOLDSTEIN_PRICE,
	BOOTH,
	BUKIN,
	MATYAS,
	ACKLEY,
	LEVI,
	TREE_HUMP_CAMMEL,
	EASOM,
	CROSS_TRAY,
	EGGHOLDER,
	HOLDER_TABLE,
	MC_CORMICK,
	SHAFFER_N2,
	SHAFFER_N4,
	STYBINSKI_TANG
};

static const double pi = 3.14159265359;
static const double e = 2.7182818284;
double rossenbrock(double x, double y)
{
	return ((1 - x)*(1 - x) + 100*( y - x*x)*(y - x*x));
}

double rastigrin(double x, double y)
{
	return ((x*x + y*y) + 25 * (std::sin(x)*std::sin(x) + std::cos(y)*std::cos(y)));
}

double paraboloid(double x, double y)
{
	return x*x + y*y;
}

double rossenbrockv(glm::vec2 v)
{
	return rossenbrock(v.x,v.y);
}

double rastigrinv(glm::vec2 v)
{
	return rastigrin(v.x,v.y);
}

double parabloloidv(glm::vec2 v)
{
	return paraboloid(v.x, v.y);
}

double fourPicksInverse(double x, double y)
{
	return -(std::abs(x) + std::abs(y))*std::exp(-0.0625*(x*x + y*y));
}

double fourPeaksInverse(double x, double y)
{
	return -(exp(-(x-4)*(x - 4) - (y - 4)*(y - 4)) + exp(-(x+4)*(x + 4) - (y-4)*(y-4)) + 2*(exp(-x*x-y*y)+exp(-x*x-(y+4)*(y+4))));
}

double baele(double x, double y)
{
	return ((1.5 - x + x*y)*(1.5 - x*x*y) + (2.25 - x + x*y*y)*(2.25 - x + x*y*y) + (2.625 - x + x*y*y*y)*(2.625 - x + x*y*y*y));
}

double goldstein_price(double x, double y)
{
	return ((1 + (x + y + 1)*(x + y + 1)*(19 - 14 * x + 3 * x*x - 14 * y + 6 * x*y + 3 * y*y))*(30 + (2 * x - 3 * y)*(2 * x - 3 * y)*(18 - 32 * x + 12 * x*x + 48 * y - 36 * x*y + 27 * y*y)));
}

double booth(double x, double y)
{
	return ((x + 2 * y - 7)*(x + 2 * y - 7) + (2 * x + y + 5)*(2 * x + y + 5));
}

double bukin(double x, double y)
{
	return (100 * std::sqrt(std::abs(y - 0.01*x*x)) + 0.01*std::abs(x + 10));
}

double matyas(double x, double y)
{
	return (0.26*(x*x + y*y) - 0.48*x*y);
}

double ackley(double x, double y)
{
	return (-20 * std::exp(-0.2*std::sqrt(0.5*(x*x + y*y))) - std::exp(0.5*(std::cos(2 * pi*x) + std::cos(2 * pi*y))) + 20 + e);
}

double levi(double x, double y)
{
	return (std::pow(std::sin(3 * pi*x), 2) + std::pow(x - 1, 2)*(1 + std::pow(std::sin(3 * pi*y), 2)) + std::pow(y - 1, 2)*(1 + std::pow(std::sin(2 * pi*y), 2)));
}

double tree_hump_cammel(double x, double y)
{
	double x2 = x*x;
	double x4 = x2*x2;
	return 2 * x2 - 1.05*x4 + x4*x2 / 6 + x*y + y*y;
}

double easom(double x, double y)
{
	return -std::cos(x)*std::cos(y)*std::exp(-(std::pow(x - pi, 2) + std::pow(y - pi, 2)));
}

double cross_tray(double x, double y)
{
	return -0.0001*std::pow((std::abs(std::sin(x)*std::sin(y)*std::exp(std::abs(100 - std::sqrt(x*x + y*y) / pi)))) + 1, 0.1);
}

double eggholder(double x, double y)
{
	return -(y + 47)*std::sin(std::sqrt(std::sqrt(abs(x / 2 + y + 47)))) - x*std::sin(std::sqrt(std::abs(x - y - 47)));
}

double holder_table(double x, double y)
{
	return -std::abs(std::sin(x)*std::cos(y)*std::exp(std::abs(1 - std::sqrt(x*x + y*y) / pi)));
}

double mcCormick(double x, double y)
{
	return (std::sin(x + y) + std::pow(x - y, 2) - 1.0*x + 2.5*y + 1);
}

double schafferN2(double x, double y)
{
	double x2 = x*x, y2 = y*y;
	return (0.5 + (std::pow(std::sin(x2 - y2), 2) - 0.5) / std::pow(1 + 0.001*(x2 + y2), 2));
}

double schafferN4(double x, double y)
{
	double x2 = x*x, y2 = y*y;
	return (0.5 + (std::pow(std::cos(std::sin(std::abs(x2-y2))),2)-0.5)/ std::pow(1 + 0.001*(x2 + y2), 2));
}

double stybinski_tang(double x, double y)
{
	double x2 = x*x, y2 = y*y;
	return ((x2*x2 - 16 * x2 + 5 * x) + (y2*y2 - 16 * y2 + 5 * y) / 2);
}

std::tuple<std::function<double(double, double)>,glm::vec2,glm::vec2> getFunction(Functions function)
{
	switch (function)
	{
	case Functions::ACKLEY:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(ackley),glm::vec2(-5),glm::vec2(5));
	case Functions::BAEALE:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(baele),glm::vec2(-4.5),glm::vec2(4.5));
	case Functions::BOOTH:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(booth),glm::vec2(-10),glm::vec2(10));
	case Functions::BUKIN:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(bukin),glm::vec2(-15,-3),glm::vec2(-5,3));
	case Functions::CROSS_TRAY:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(cross_tray),glm::vec2(-10),glm::vec2(10));
	case Functions::EASOM:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(easom),glm::vec2(-25),glm::vec2(25));
	case Functions::EGGHOLDER:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(eggholder),glm::vec2(-512),glm::vec2(512));
	case Functions::FOUR_PEAKS_INVERSE:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(fourPeaksInverse),glm::vec2(-10),glm::vec2(10));
	case Functions::FOUR_PICK_INVERSE:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(fourPicksInverse),glm::vec2(-10),glm::vec2(10));
	case Functions::GOLDSTEIN_PRICE:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(goldstein_price),glm::vec2(-2),glm::vec2(2));
	case Functions::HOLDER_TABLE:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(holder_table),glm::vec2(-10),glm::vec2(10));
	case Functions::LEVI:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(levi),glm::vec2(-10),glm::vec2(10));
	case Functions::MATYAS:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(matyas),glm::vec2(-10),glm::vec2(10));
	case Functions::MC_CORMICK:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(mcCormick),glm::vec2(-1.5,4),glm::vec2(-3,4));
	case Functions::PARABOLOID:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(paraboloid),glm::vec2(-5),glm::vec2(5));
	case Functions::RASTIGIN:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(rastigrin),glm::vec2(-5),glm::vec2(5));
	case Functions::ROSENBROCK:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(rossenbrock),glm::vec2(-5),glm::vec2(5));
	case Functions::SHAFFER_N2:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(schafferN2),glm::vec2(-50),glm::vec2(50));
	case Functions::SHAFFER_N4:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(schafferN4), glm::vec2(-50), glm::vec2(50));;
	case Functions::STYBINSKI_TANG:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(stybinski_tang),glm::vec2(-5),glm::vec2(5));
	case Functions::TREE_HUMP_CAMMEL:
		return std::make_tuple<std::function<double(double, double)>, glm::vec2, glm::vec2>(std::ref(tree_hump_cammel),glm::vec2(-5),glm::vec2(5));
	default:
		break;
	}
}

#endif // !FUNCTIONS_H
