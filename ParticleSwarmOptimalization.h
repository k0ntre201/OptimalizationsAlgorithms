#ifndef PARTICLE_SWARM_OPTIMALIZATION_H
#define PARTICLE_SWARM_OPTIMALIZATION_H

#include <SDL\SDL.h>
#include <glm\glm.hpp>
#include <valarray>
#include <random>
#include "MyRandom.h"
#include <chrono>
#include <memory>

struct Particles
{
	glm::vec2 pos;
	glm::vec2 vel;
	double f;
};

class ParticleSwarmOptimalization
{
public:
	ParticleSwarmOptimalization(const glm::vec2& s, const glm::vec2& x1, const glm::vec2& x2, std::size_t N = 1000, double w = 0., double alpha = 2.0, double beta = 2.0);
	virtual ~ParticleSwarmOptimalization();

	void run();

private:
	double paraboloid(const glm::vec2& v,const glm::vec2& mousepos);
	void pso();
	void drawParticles();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	glm::vec2 div,s,x1,x2,g;
	std::size_t N;
	double w,alpha,beta,fxy;
	std::valarray<Particles> particles;
	std::valarray<glm::vec2> p;
	std::valarray<glm::ivec2> dr;
	std::default_random_engine engine;
	TwoDimensionUniformRandom dist1;
	TwoDimensionUniformRandom dist2;
};

#endif // !PARTICLE_SWARM_OPTIMALIZATION_H
