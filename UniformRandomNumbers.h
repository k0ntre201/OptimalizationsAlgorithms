#ifndef UNIFORM_RANDOM_NUMBERS_H
#define UNIFORM_RANDOM_NUMBERS_H

#include "MyRandom.h"
#include <SDL\SDL.h>
#include <glm\glm.hpp>
#include <chrono>

class UniformRandomNumbers
{
public:
	UniformRandomNumbers(const glm::ivec2& windowPos = glm::ivec2(20), const glm::vec2& windowSize = glm::ivec2(300))
	{
		size = windowSize;
		generator = MyRandom::basic_linear_generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		dist = MyRandom::uniform_integer_distribution<int>(0, size.x);
		window = SDL_CreateWindow("uniform linear generator", windowPos.x, windowPos.y, size.x, size.y, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}

	void run()
	{
		int timerFps;
		SDL_Event e;
		bool quit = false;
		int t = 0;
		bool end;
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 0);
		while (!quit)
		{
			timerFps = SDL_GetTicks();
			if (SDL_PollEvent(&e))
				if (e.type == SDL_QUIT)
					quit = true;
			glm::ivec2 pos = glm::ivec2(dist(generator), dist(generator));
			SDL_RenderDrawPoint(renderer, pos.x, pos.y);
			SDL_RenderPresent(renderer);
			timerFps = SDL_GetTicks() - timerFps;
			if (timerFps < 1000 / 30)
			{
				SDL_Delay((1000 / 30) - timerFps);
			}
			//std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	}

	~UniformRandomNumbers()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}
private:
	glm::ivec2 size;
	MyRandom::basic_linear_generator generator;
	MyRandom::uniform_integer_distribution<int> dist;
	SDL_Window* window;
	SDL_Renderer* renderer;
};
#endif // !UNIFORM_RANDOM_NUMBERS_H
