#include <iostream>
#include "functions.h"
#include "optimalizationAlgorithms.h"

int main()
{
	double x, y, ef;
	simulatedAnneaning(x, y, ef, rastigrin);

	std::cout << x << " " << y << " " << ef << "\n";

	std::cin.get();
	return 0;
}