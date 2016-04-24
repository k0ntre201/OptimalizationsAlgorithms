#ifndef DRAWING_CLASS_H
#define DRAWING_CLASS_H

#include <SDL\SDL.h>
#include "GenerateFucntionTexture.h"
#include <glm\glm.hpp>
#include <array>
#include <SDL\SDL_ttf.h>
#include <string>

class DrawingClass
{
public:
	template<typename F>
	DrawingClass(F f,const glm::vec2& v1, const glm::vec2& v2)
	{
		window = SDL_CreateWindow("Minimum", 20, 20, textureSize.x+200, textureSize.y+100, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		matrix = Matrix<double>(textureSize.x, textureSize.y);
		generateFunctionTexture(matrix, f, textureSize, v1, v2,max,min);
		this->v1 = v1;
		interval = glm::abs(v2 - v1);
		font = TTF_OpenFont("AGENCYR.ttf", 20);
		createTextures();
	}

	void drawPoinstOnFunction(std::valarray<glm::vec2> points)
	{
		for (int i = 0; i < points.size(); ++i)
		{
			points[i] = ((points[i] - v1 )/ interval )*glm::vec2(textureSize);
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		for (int i = 0; i < textureSize.x; ++i)
		{
			for (int j = 0; j < textureSize.y; ++j)
			{
				double color = matrix.at(i, j);
				if (color > 255)
				SDL_SetRenderDrawColor(renderer, color - 255, 510 - color, 0, 0);
				else
					SDL_SetRenderDrawColor(renderer, 0, color,255- color, 0);
				SDL_RenderDrawPoint(renderer, i+45, j+20);
			}
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 200, 0);
		for (int i = 0; i < points.size(); ++i)
		{
			SDL_Rect rect{ points[i].x - 2 + 45,points[i].y+20 - 2,4,4 };
			SDL_RenderFillRect(renderer, &rect);
		}

		drawCoordinateSystem();
		drawLegend();
		SDL_RenderPresent(renderer);
	}

	~DrawingClass()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		//if (font != nullptr)
			//TTF_CloseFont(font);
	}
private:
	void drawCoordinateSystem()
	{
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 0);
		SDL_RenderDrawLine(renderer, 45, 20, 45, textureSize.y + 45);
		SDL_RenderDrawLine(renderer, 25, textureSize.y + 20, textureSize.x + 45, textureSize.y + 20);
		for (int i = 0; i <= 10; ++i)
		{
			SDL_RenderDrawLine(renderer, 25, i * 60 + 20, 45, i * 60 + 20);
			SDL_RenderDrawLine(renderer, i * 60 + 45, textureSize.y + 20, i * 60 + 45, textureSize.y + 35);
			SDL_Rect rect1 = { 0,i * 60 + 10,35,20 };
			SDL_Rect rect2 = { i * 60 + 20, textureSize.y + 35,35,20 };
			SDL_RenderCopy(renderer, numbers[i], NULL, &rect1);
			SDL_RenderCopy(renderer, numbers[i + 11], NULL, &rect2);
			if (i < 10)
			{
				for (int j = 1; j <= 3; ++j)
				{
					SDL_RenderDrawLine(renderer, 35, i * 60 + j * 15 + 20, 45, i * 60 + j * 15 + 20);
					SDL_RenderDrawLine(renderer, i * 60 + j * 15 + 45, textureSize.y + 20, i * 60 + j * 15 + 45, textureSize.y + 25);
				}
			}
		}
	}

	void createTextures()
	{
		glm::vec2 offset = interval / glm::vec2(10);
		glm::vec2 vtemp = v1;
		for (int i = 0; i <= 10; ++i)
		{
			std::string digit1 = std::to_string(vtemp.x);
			std::string digit2 = std::to_string(vtemp.y);
			while (digit1.size()>5)
			{
				digit1.pop_back();
			}
			while (digit2.size()>5)
			{
				digit2.pop_back();
			}

			SDL_Surface* surf1 = TTF_RenderText_Solid(font, digit1.c_str(), { 255,255,255 });
			SDL_Surface* surf2 = TTF_RenderText_Solid(font, digit2.c_str(), { 255,255,255 });
			numbers[i] = SDL_CreateTextureFromSurface(renderer, surf1);
			numbers[i + 11] = SDL_CreateTextureFromSurface(renderer, surf2);
			vtemp += offset;
		}
		std::string minDig = std::to_string(min);
		std::string maxDig = std::to_string(max);
		SDL_Surface* surf1 = TTF_RenderText_Solid(font, minDig.c_str(), { 255,255,255 });
		SDL_Surface* surf2 = TTF_RenderText_Solid(font, maxDig.c_str(), { 255,255,255 });
		minTex = SDL_CreateTextureFromSurface(renderer, surf1);
		maxTex = SDL_CreateTextureFromSurface(renderer, surf2);
	}

	void drawLegend()
	{
		for (int y = 0; y < 510; y+=2)
		{
			if (y > 255)
				SDL_SetRenderDrawColor(renderer, y - 255, 510 - y, 0, 0);
			else
				SDL_SetRenderDrawColor(renderer, 0, y, 255 - y, 0);
			SDL_RenderDrawLine(renderer, 650, -y/2 + 305, 700, -y/2 + 305);
		}
		SDL_Rect rect1 = { 710,55,90,20 };
		SDL_Rect rect2 = { 710,285,90,20 };
		SDL_RenderCopy(renderer, minTex, NULL, &rect2);
		SDL_RenderCopy(renderer, maxTex, NULL, &rect1);
	}

private:
	Matrix<double> matrix;
	double min, max;
	glm::ivec2 textureSize = glm::ivec2(600);
	SDL_Window* window;
	SDL_Renderer* renderer;
	glm::vec2 interval, v1;
	TTF_Font* font;
	std::array<SDL_Texture*, 22> numbers;
	SDL_Texture* minTex, *maxTex;
};
#endif // !DRAWING_CLASS_H
