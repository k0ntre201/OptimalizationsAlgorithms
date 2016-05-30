#ifndef DRAW_FOUR_GENERATORS_H
#define DRAW_FOUR_GENERATORS_H

#include "UniformRandomNumbers.h"
#include "CauchyRandomNumbers.h"
#include "GaussRandomNumbers.h"
#include "LevyRandomNumbers.h"
#include <thread>

class DrawFourGenerators
{
public:
	void run()
	{
		UniformRandomNumbers uniformRandomNumbers;
		CauchyRandomNumbers cauchyRandomNumbers(glm::ivec2(400, 20));
		GaussRandomNumbers gaussRandomNumbers(glm::ivec2(20, 400));
		LevyRandomNumbers levyRandomNumbers(glm::ivec2(400, 400));
		std::thread th1(&UniformRandomNumbers::run, uniformRandomNumbers);
		std::thread th2(&CauchyRandomNumbers::run, cauchyRandomNumbers);
		std::thread th3(&GaussRandomNumbers::run, gaussRandomNumbers);
		std::thread th4(&LevyRandomNumbers::run, levyRandomNumbers);
		th1.join();
		th2.join();
		th3.join();
		th4.join();
	}
};
#endif // !DRAW_FOUR_GENERATORS_H

