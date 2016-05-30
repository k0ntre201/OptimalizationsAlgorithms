#ifndef SIMULATED_ANEANING_H
#define SIMULATED_ANEANING_H

#include <memory>
#include "DrawingClass.h"
#include "functions.h"
#include <functional>
#include "MyRandom.h"
#include <chrono>
#include <random>
#include <thread>

class SilumatedAneaning
{
public:
	
	SilumatedAneaning(std::function<double(double,double)> f, const glm::vec2& v1, const glm::vec2& v2, double T0 = 800.0, double Tf = 10e-6, double alpha = 0.999)
	{
		this->T0 = T0;
		this->Tf = Tf;
		this->alpha = alpha;
		fun = f;
		drawingClass.reset(new DrawingClass(fun, v1, v2));
		this->v1 = v1;
		this->v2 = v2;
	}



	void run()
	{
		std::mt19937 engine(std::chrono::system_clock::now().time_since_epoch().count());
		TwoDimensionUniformRandom dist(v1, v2);
		TwoDimensionUniformRandom dist2(glm::vec2(-1, -1), glm::vec2(1, 1));
		std::uniform_real_distribution<double> dist3(0.0, 1.0);
		v = dist(engine);
		double fxy = fun(v.x,v.y);

		SDL_Event e;
		quit = false;
		int i = 0;
		bool end;
		while (!quit)
		{
			if (SDL_PollEvent(&e))
				if (e.type == SDL_QUIT)
					quit = true;
			end = true;
			if (T0 > Tf)
			{
				end = false;
				glm::vec2 temp = dist(engine);
				glm::vec2 nV = v + temp;
				if (nV.x<v1.x || nV.x>v2.x)
					nV.x = temp.x;
				if (nV.y<v1.y || nV.y>v2.y)
					nV.y = temp.y;
				double nextFx = fun(nV.x, nV.y);
				double delta = nextFx - fxy;
				if (delta < 0)
				{
					v = nV;
					fxy = nextFx;
				}
				else
				{
					double r = dist3(engine);
					if (r < std::exp(-delta / T0))
					{
						v = nV;
						fxy = nextFx;
					}
				}
				T0 *= alpha;
				++i;
				//std::cout << i << "\n";
			}
			if (i % 250 == 0 || end == true)
			{
				std::cout << i << "\n";
				drawingClass->drawPoinstOnFunction({ v });
				std::cout << v.x << " " << v.y << "\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
		
	}
private:

private:
	std::unique_ptr<DrawingClass> drawingClass;
	double T0, Tf, alpha;
	std::function<double(double,double)> fun;
	glm::vec2 v,v1,v2;
	bool quit;
};

#endif // !SIMULATED_ANEANING_H
