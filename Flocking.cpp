#include "Flocking.h"
#include <iostream>

void Flocking::flock()
{
	
	for (int i = 0; i < boids.size(); ++i)
	{
		separate(i,40,0.025);
		aligment(i,80,0.025);
		cohesion(i,80,0.01);
	}
}

void Flocking::wander(double multipler)
{
	//wanderTheta = wanderPsi = 0;
	for (int i = 0; i < boids.size(); ++i)
	{
		wanderTheta += temp(engine);
		wanderPsi +=temp(engine);
		//if (temp(engine) < 0.5)
			//wanderTheta = -wanderTheta;

		glm::vec2 pos = boids[i].velliocity;
		
		pos = glm::normalize(pos);
		pos *= glm::vec2(wanderSteep);
		glm::vec2 offset = glm::vec2(wanderRadius*temp(engine), wanderRadius*temp(engine));
		glm::vec2 target = pos + offset;
		//std::cout << target.x << " " << target.y << "\n";
		//std::cin.get();
		boids[i].steeringForce = steer(boids[i],target);
		boids[i].steeringForce += glm::vec2(0.7);
		boids[i].steeringForce *= multipler;
		boids[i].acceleration += boids[i].steeringForce;
		
	}
}

void Flocking::update()
{
	for (int i = 0; i < boids.size(); ++i)
	{
		boids[i].oldPositon = boids[i].position;
		boids[i].velliocity += boids[i].acceleration;
		double lengthSq = glm::length(boids[i].velliocity);
		lengthSq = lengthSq*lengthSq;
		if (lengthSq > maxSpeedSQ)
		{
			boids[i].velliocity = glm::normalize(boids[i].velliocity);
			boids[i].velliocity *= glm::vec2(maxSpeed);
		}
		boids[i].position += boids[i].velliocity;
		boids[i].acceleration = glm::vec2(0);
		if(boids[i].position!=boids[i].oldPositon)
			constrainToRectangle(i);
	}
}

void Flocking::seek(const glm::vec2 & target, double multipler)
{
	for (int i = 0; i < boids.size(); ++i)
	{
		boids[i].steeringForce = steer(boids[i], target);
		boids[i].steeringForce *= glm::vec2(multipler);
		boids[i].acceleration += boids[i].steeringForce;
	}
}

void Flocking::render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
	for (int i = 0; i < boids.size(); ++i)
	{
		SDL_Rect rect;
		rect.x = boids[i].position.x - 5;
		rect.y = boids[i].position.y - 5;
		rect.w = 10;
		rect.h = 10;
		SDL_RenderDrawRect(renderer, &rect);
		//SDL_RenderDrawPoint(renderer, boids[i].position.x, boids[i].position.y);
	}
	SDL_RenderPresent(renderer);
}

void Flocking::run()
{
	initializeBoids();
	SDL_Event e;
	bool quit = false;
	int timerFps;
	int x, y;
	while (!quit)
	{
		timerFps = SDL_GetTicks();
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				quit = true;
		
		flock();
		update();
		
		timerFps = SDL_GetTicks() - timerFps;
		if (timerFps < 1000 / 30) 
		{
			SDL_Delay((1000 / 30) - timerFps);
		}
		render();
		wander(0.3);
		//seek(glm::vec2(width / 2, height / 2), 0.1);
		//SDL_GetMouseState(&x, &y);
		//seek(glm::vec2(x, y), 0.5);
	}
}

void Flocking::freeFlocking()
{
	initializeBoids();
	SDL_Event e;
	bool quit = false;
	int timerFps;
	int x, y;
	while (!quit)
	{
		timerFps = SDL_GetTicks();
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				quit = true;

		for (int i = 0; i < boids.size(); ++i)
		{
			separate(i, 40, 0.25);
			aligment(i, 80, 0.25);
			cohesion(i, 80, 0.1);
		}
		update();
		render();
		timerFps = SDL_GetTicks() - timerFps;
		if (timerFps < 1000 / 30)
		{
			SDL_Delay((1000 / 30) - timerFps);
		}
	}
}

void Flocking::wandererFlocking()
{
	initializeBoids();
	SDL_Event e;
	bool quit = false;
	int timerFps;
	int x, y;
	while (!quit)
	{
		timerFps = SDL_GetTicks();
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				quit = true;

		flock();
		update();
		render();
		wander(0.1);

		timerFps = SDL_GetTicks() - timerFps;
		if (timerFps < 1000 / 30)
		{
			SDL_Delay((1000 / 30) - timerFps);
		}
	}
}

void Flocking::flockingMouse()
{
	initializeBoids();
	SDL_Event e;
	bool quit = false;
	int timerFps;
	int x, y;
	while (!quit)
	{
		timerFps = SDL_GetTicks();
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				quit = true;

		flock();
		update();
		render();
		SDL_GetMouseState(&x, &y);
		if(e.type == SDL_MOUSEBUTTONDOWN)
			seek(glm::vec2(x, y), -0.2);
		else
			seek(glm::vec2(x, y), 0.2);
		
		timerFps = SDL_GetTicks() - timerFps;
		if (timerFps < 1000 / 30)
		{
			SDL_Delay((1000 / 30) - timerFps);
		}
	}
}

void Flocking::wandererMouseFlocking()
{
	initializeBoids();
	SDL_Event e;
	bool quit = false;
	int timerFps;
	int x, y;
	while (!quit)
	{
		timerFps = SDL_GetTicks();
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				quit = true;

		flock();
		update();

		
		render();
		wander(0.1);
		SDL_GetMouseState(&x, &y);
		if (e.type == SDL_MOUSEBUTTONDOWN)
			seek(glm::vec2(x, y), -0.2);
		else
			seek(glm::vec2(x, y), 0.2);

		timerFps = SDL_GetTicks() - timerFps;
		if (timerFps < 1000 / 30)
		{
			SDL_Delay((1000 / 30) - timerFps);
		}
	}
}

glm::vec2 Flocking::steer(Boid & boid, const glm::vec2 & target, double easeDistance)
{
	boid.steeringForce = target;
	boid.steeringForce -= boid.position;
	double distance = glm::length(boid.steeringForce);
	if (distance > 10)
	{
		if (distance < easeDistance)
			boid.steeringForce *= glm::vec2(maxSpeed*(distance / easeDistance));
		else
			boid.steeringForce *= glm::vec2(maxSpeed);

		boid.steeringForce -= boid.velliocity;
		if (distance*distance>maxForceSQ)
		{
			boid.steeringForce = glm::normalize(boid.steeringForce);
			boid.steeringForce *= glm::vec2(maxForce);
		}
	}
	return boid.steeringForce;
}

void Flocking::cohesion(int indx, double neighborDistance, double multipler)
{
	int count = 0;
	glm::vec2 force = glm::vec2(0);
	double distance = 0;
	Boid boid;
	for (int i = 0; i < boids.size(); ++i)
	{
		if (i != indx)
		{
			distance = glm::distance(boids[indx].position, boids[i].position);
			if (distance > 0 && distance < neighborDistance)
			{
				force += boids[i].position;
				++count;
			}
		}
	}
	if (count > 0)
	{
		force *= glm::vec2(1.0 / (double)count);
		force = steer(boids[indx], force);
	}
	boids[indx].steeringForce = force;
	boids[indx].steeringForce *= glm::vec2(multipler);
	boids[indx].acceleration += boids[indx].steeringForce;
}

void Flocking::separate(int indx, double neighborDistance, double multipler)
{
	glm::vec2 force = glm::vec2(0);
	glm::vec2 difference = glm::vec2(0);
	double distance = 0;
	int count = 0;
	for (int i = 0; i < boids.size(); ++i)
	{
		if (i!= indx)
		{
			distance = glm::distance(boids[indx].position, boids[i].position);
			if (distance > 0 && distance < neighborDistance)
			{
				difference = boids[indx].position - boids[i].position;
				difference = glm::normalize(difference);
				force += difference;
				++count;
			}
		}
	}
	if (count > 0)
		force *= glm::vec2(1.0 / (double)count);
	boids[indx].steeringForce = force;
	boids[indx].steeringForce *= glm::vec2(multipler);
	boids[indx].acceleration += boids[indx].steeringForce;
}

void Flocking::aligment(int indx, double neighborDistance, double multipler)
{
	glm::vec2 force = glm::vec2(0);
	double distance = 0;
	int count = 0;
	for (int i = 0; i < boids.size(); ++i)
	{
		if (i != indx)
		{
			distance = glm::distance(boids[i].position, boids[indx].position);
			if (distance > 0 && distance < neighborDistance)
			{
				force += boids[i].velliocity;
				++count;
			}
		}
	}

	if (count > 0)
	{
		force *= glm::vec2(1.0 / (double)count);
		double lengthSq = glm::length(force);
		lengthSq *= lengthSq;
		if (lengthSq > maxForceSQ)
		{
			force = glm::normalize(force);
			force *= glm::vec2(maxForce);
		}
	}
	boids[indx].steeringForce = force;
	boids[indx].steeringForce *= glm::vec2(multipler);
	boids[indx].acceleration += boids[indx].steeringForce;
}

void Flocking::constrainToRectangle(int indx)
{
	if (boids[indx].position.x < radius)
	{
		boids[indx].position.x = radius;
		boids[indx].velliocity.x *= -1;
	}
	else if (boids[indx].position.x > width - radius)
	{
		boids[indx].position.x = width - radius;
		boids[indx].velliocity.x *= -1;
	}
	else if (boids[indx].position.y < radius)
	{
		boids[indx].position.y = radius;
		boids[indx].velliocity.y *= -1;
	}
	else if (boids[indx].position.y > height - radius)
	{
		boids[indx].position.y = height - radius;
		boids[indx].velliocity.y *= -1;
	}
}

void Flocking::initializeBoids()
{
	for (int i = 0; i < boids.size(); ++i)
	{
		boids[i].reset();
		boids[i].position = distribution(engine);
		boids[i].velliocity.x = temp(engine);
		boids[i].velliocity.y = temp(engine);
	}
}
