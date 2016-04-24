#ifndef FLOCKING_H
#define FLOCKING_H

#include "Boid.h"
#include <valarray>
#include <random>
#include <chrono>
#include "MyRandom.h"
#include <SDL\SDL.h>

class Flocking
{
public:
	Flocking(int n,int w,int h)
	{
		boids.resize(n);
		width = w;
		height = h;
		engine = std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		distribution = TwoIntegerDimensionUniformRandom(glm::ivec2(0), glm::ivec2(w, h));
		temp = std::uniform_real_distribution<double>(-wanderSteep,wanderSteep);
		window = SDL_CreateWindow("flocking", 20, 20, w, h, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}
	void run();
private:
	std::valarray<Boid> boids;
	int width, height;
	double maxSpeed = 6.0;
	double maxForce = 1.0;
	double maxSpeedSQ = maxSpeed*maxSpeed;
	double maxForceSQ = maxForce*maxForce;
	double boundsRadius = 10;
	glm::vec2 boundsCentre;
	double radius = 10.0;
	double wanderTheta = 0, wanderPsi = 0;
	double wanderRadius = 60;
	double wanderDistace = 16;
	double wanderSteep = 0.25;
	std::default_random_engine engine;
	TwoIntegerDimensionUniformRandom distribution;
	std::uniform_real_distribution<double> temp;
	SDL_Window* window;
	SDL_Renderer* renderer;
private:
	glm::vec2 steer(Boid& boid,const glm::vec2& target,double easeDistance = 10);
	void cohesion(int indx,double neighborDistance = 10,double multipler = 1);
	void separate(int indx, double neighborDistance = 40, double multipler = 1);
	void aligment(int indx, double neighborDistance = 50, double multipler = 1);
	void constrainToRectangle(int indx);
	void initializeBoids();
	void flock();
	void wander(double multipler);
	void update();
	void seek(const glm::vec2& target,double multipler);
	void render();
};

#endif // !FLOCKING_H
