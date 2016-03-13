#ifndef MY_RANDOM_H
#define MY_RANDOM_H

#include <glm\glm.hpp>
#include <random>




class TwoDimensionUniformRandom
{
public:
	TwoDimensionUniformRandom(glm::vec2 v1, glm::vec2 v2)
	{
		dx = std::uniform_real_distribution<float>(v1.x, v2.x);
		dy = std::uniform_real_distribution<float>(v1.y, v2.y);
	}
	~TwoDimensionUniformRandom() {}
	template<typename Engine>
	glm::vec2 operator()(Engine& engine)
	{
		return glm::vec2(dx(engine), dy(engine));
	}
private:
	std::uniform_real_distribution<float> dx, dy;
};

#endif // !MY_RANDOM_H

