#include <iostream>
#include "functions.h"
#include "optimalizationAlgorithms.h"

int main()
{
	double x, y, ef;
	simulatedAnneaning(x, y, ef, rossenbrock);
	glm::vec2 v;
	double fxy;
	simulatedAnneaning(v, fxy, glm::vec2(-5, -5), glm::vec2(5, 5), rossenbrockv);

	std::cout << x << " " << y << " " << ef << "\n";
	std::cout << v.x << " " << v.y << " " << fxy << "\n";
	std::cin.get();
	return 0;
}