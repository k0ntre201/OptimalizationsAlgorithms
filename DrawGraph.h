#ifndef DRAW_GRAPH_H
#define DRAW_GRAPH_H

#include <SDL\SDL.h>
#include "Vertex.h"

template<typename Vert>
class DrawGraph
{
public:
	
	DrawGraph(const std::vector<Vert>& vert)
	{
		window = SDL_CreateWindow("graph", 20, 20, 700, 500, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		this->vert = vert;
		//std::cout << vert.size() << "\n";
		//std::cout << this->vert.size() << "\n";
	}

	void draw(const std::vector<int>& bestPath)
	{	
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		
		for (int i = 0; i < vert.size(); ++i)
		{
			glm::vec2 position = vert[i].position;
			SDL_Rect rect;
			rect.x = position.x - 10;
			rect.y = position.y - 10;
			rect.h = rect.w = 20;
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
			SDL_RenderFillRect(renderer, &rect);
			for (int j = 0; j < vert[i].neighbors.size(); ++j)
			{
				glm::vec2 position2 = vert[vert[i].neighbors[j].index].position;
				SDL_RenderDrawLine(renderer, position.x, position.y, position2.x, position2.y);
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		for (int i = 0; i < bestPath.size()-1; ++i)
		{
			glm::vec2 position = vert[bestPath[i]].position;
			glm::vec2 position2 = vert[bestPath[i+1]].position;
			SDL_Rect rect;
			rect.x = position.x - 10;
			rect.y = position.y - 10;
			rect.h = rect.w = 20;
			SDL_RenderFillRect(renderer, &rect);
			SDL_RenderDrawLine(renderer, position.x, position.y, position2.x, position2.y);
		}
		glm::vec2 position = vert[vert.size()-1].position;
		SDL_Rect rect;
		rect.x = position.x - 10;
		rect.y = position.y - 10;
		rect.h = rect.w = 20;
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderPresent(renderer);
	}

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::vector<Vert> vert;
};

#endif //!DRAW_GRAPH