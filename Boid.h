#ifndef BOID_H
#define BOID_H

#include <glm\glm.hpp>

struct Boid
{
	glm::vec2 oldPositon,position, velliocity, acceleration,steeringForce,wanderTheta,wanderPhi;
	void reset()
	{
		oldPositon = position = velliocity = acceleration = steeringForce = steeringForce = wanderPhi = glm::vec2(0);
	}
};

#endif // !BOID_H
