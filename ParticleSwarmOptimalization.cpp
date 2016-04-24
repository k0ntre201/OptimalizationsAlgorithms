#include "ParticleSwarmOptimalization.h"
#include <iostream>

ParticleSwarmOptimalization::ParticleSwarmOptimalization(const glm::vec2 & s, const glm::vec2 & x1, const glm::vec2 & x2, std::size_t N, double w, double alpha, double beta)
{
	engine = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
	dist1 = (TwoDimensionUniformRandom(x1, x2));
	dist2= (TwoDimensionUniformRandom(glm::vec2(0.0), glm::vec2(1.0)));
	this->s = s;
	this->x1 = x1;
	this->x2 = x2;
	this->alpha = alpha;
	this->beta = beta;
	this->N = N;
	this->w = w;
	window = SDL_CreateWindow("PSO", 20, 20, s.x, s.y, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	particles.resize(N);
	p.resize(N);
	g = (dist1(engine));
	dr.resize(N);
	div = (x2 - x1) / s;

	for (int i = 0; i < N; ++i)
	{
		p[i] = particles[i].pos = dist1(engine);
		particles[i].vel = glm::vec2(0.0);
		if (paraboloid(p[i], s*glm::vec2(0.5) *div - glm::vec2(0.5)) < paraboloid(g, s*glm::vec2(0.5) *div - glm::vec2(0.5)))
			g = p[i];
	}
}

ParticleSwarmOptimalization::~ParticleSwarmOptimalization()
{
}

void ParticleSwarmOptimalization::run()
{
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				quit = true;
		pso();
		drawParticles();
	}
}

double ParticleSwarmOptimalization::paraboloid(const glm::vec2 & v, const glm::vec2& mousepos)
{
	return (v.x - mousepos.x)*(v.x - mousepos.x) + (v.y - mousepos.y)*(v.y - mousepos.y);
}

void ParticleSwarmOptimalization::pso()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	//std::cout << x << " " << y << "\n";
	glm::vec2 position = (glm::vec2(x, y) / s + glm::vec2(0.5f))*(x2 - x1);
	for (int i = 0; i < N; ++i)
	{
		glm::vec2 rp = dist2(engine);
		glm::vec2 rg = dist2(engine);
		particles[i].vel = (particles[i].vel*glm::vec2(w) + rp*(p[i] - particles[i].pos)*glm::vec2(alpha) +rg*(g - particles[i].pos)*glm::vec2(beta));
		particles[i].pos += particles[i].vel;
		if (paraboloid(particles[i].pos, position) < paraboloid(p[i], position))
		{
			p[i] = particles[i].pos + (dist2(engine)*glm::vec2(0.125f) - glm::vec2(0.0625));
			if (paraboloid(p[i], position) < paraboloid(g, position))
			{
				g = p[i] + (dist2(engine)*glm::vec2(0.25f) - glm::vec2(0.125));
				fxy = paraboloid(g, position);
			}
		}
	}
}

void ParticleSwarmOptimalization::drawParticles()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	for (int i = 0; i < N; ++i)
		dr[i] = ((p[i] / (x2 - x1) - glm::vec2(0.5) )*s);

	
	for (int i = 0; i < N; ++i)
	{
		SDL_SetRenderDrawColor(renderer, dr[i].x%255, (dr[i].x + dr[i].y) % 255, dr[i].y % 255, 0);
		SDL_RenderDrawPoint(renderer, dr[i].x, dr[i].y);
		//std::cout << dr[i].x << " " << dr[i].y << "\n";
	}
	//std::cin.get();
	SDL_RenderPresent(renderer);
}
