#ifndef GENERATE_FUNCTION_TEXTURE_H
#define GENERATE_FUNCTION_TEXTURE_H

#include "Matrix.h"
#include <glm\glm.hpp>
#include "functions.h"

template<typename F>
void generateFunctionTexture(Matrix<double>& matrix, F f,const glm::ivec2& textureSize,const glm::vec2& v1,const glm::vec2& v2,double& max,double& min)
{
	double dx = (v2.x - v1.x) / textureSize.x;
	double dy = (v2.y - v1.y) / textureSize.y;
	
	double x = v1.x;
	
	for (int i = 0; i < textureSize.x; ++i)
	{
		double y = v1.y;
		for (int j = 0; j < textureSize.y; ++j)
		{
			matrix.at(i, j) = f(x, y);
			y += dy;
		}
		x += dx;
	}

	max = matrix.max();
	min = matrix.min();
	double delta = max - min;
	for (int i = 0; i < textureSize.x; ++i)
	{
		for (int j = 0; j < textureSize.y; ++j)
		{
			matrix.at(i, j) = ((matrix.at(i, j)-min) / delta) * 510;
		}
	}
}

#endif // !GENERATE_FUNCTION_TEXTURE_H
