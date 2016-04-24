//#ifndef CHARGED_SYSTEM_SEARCH_H
//#define CHARGED_SYSTEM_SEARCH_H

#include <iostream>
#include <memory>
#include "DrawingClass.h"
#include "functions.h"
#include <functional>
#include "MyRandom.h"
#include <chrono>
#include <random>
#include <thread>
#include <valarray>
#include <vector>
#include <algorithm>

class ChargedSystemSearch
{
	struct ChargedParticle
	{
		glm::vec2 position, oldPosition, veloicity;
		double q,f;
		bool operator<(const ChargedParticle& cp)
		{
			return this->q < cp.q;
		}
	};
public:
	ChargedSystemSearch(std::function<double(double, double)> fun,const glm::vec2& v1, glm::vec2& v2,int N = 20)
	{
		chargedParticles.resize(N);
		cm.resize(N / 4);
		v.resize(N);
		f = fun;
		this->v1 = v1;
		this->v2 = v2;
		epsilon = 10e-6;
		a = 0.1*std::max(std::abs(v2.x - v1.x), std::abs(v2.y - v1.y));
		random = TwoDimensionUniformRandom(v1, v2);
		random2 = TwoDimensionUniformRandom(glm::vec2(0.0), glm::vec2(1.0));
		engine = std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		drawingClass.reset(new DrawingClass(f, v1, v2));
	}

	void run(int maxIterations = 50)
	{
		initialize();
		ranking();
		createCm();

		SDL_Event e;
		bool quit = false;
		int t = 0;
		bool end;
		while (!quit)
		{
			if (SDL_PollEvent(&e))
				if (e.type == SDL_QUIT)
					quit = true;
			if (t < maxIterations)
			{
				ka = 0.5*(1 + t / maxIterations);
				kv = 0.5*(1 - t / maxIterations);
				for (int i = 0; i < chargedParticles.size(); ++i)
				{
					v[i] = chargedParticles[i].oldPosition = chargedParticles[i].position;
				}
				for (int i = 0; i < chargedParticles.size(); ++i)
				{
					updatePositionAndVeliocity(i);
					correctPosition(i);
				}
				ranking();
				actualizeCm();
				++t;
			}
			drawingClass->drawPoinstOnFunction(v);
			std::cout << t << "\n";
			std::cout << cm[0].position.x << " " << cm[0].position.x << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
private:
	double a,r,epsilon;
	glm::vec2 v1, v2;
	std::function<double(double, double)> f;
	std::valarray<ChargedParticle> chargedParticles;
	std::valarray<ChargedParticle> cm;
	std::valarray<glm::vec2> v;
	std::unique_ptr<DrawingClass> drawingClass;
	TwoDimensionUniformRandom random;
	TwoDimensionUniformRandom random2;
	std::default_random_engine engine;
	double ka, kv;
	void initialize()
	{
		for (int i = 0; i < chargedParticles.size(); ++i)
		{
			chargedParticles[i].oldPosition = chargedParticles[i].position =  random(engine);
			chargedParticles[i].veloicity = glm::vec2(0.0);
		}
		ranking();
		
	}

	void ranking()
	{
		for (int i = 0; i < chargedParticles.size(); ++i)
		{
			chargedParticles[i].f = f(chargedParticles[i].position.x, chargedParticles[i].position.y);
			chargedParticles[i].q = (chargedParticles[i].f - chargedParticles[chargedParticles.size() - 1].f) / (chargedParticles[0].f - chargedParticles[chargedParticles.size() - 1].f);
		}
		std::sort(&chargedParticles[0], &chargedParticles[0] + chargedParticles.size());
	}

	void createCm()
	{
		for (int i = 0; i < cm.size(); ++i)
			cm[i] = chargedParticles[i];
	}

	glm::vec2 determineTheImpact(int indx)
	{
		glm::vec2 F = glm::vec2(0);
		for (int j = 0; j < chargedParticles.size(); ++j)
		{
			
			if (j != indx)
			{
				//double p = ((chargedParticles[indx].f - cm[0].f) / (chargedParticles[j].f - chargedParticles[indx].f)) > r?1:0;
				double p = (chargedParticles[j].f < chargedParticles[indx].f) ? 1 : 0;
				double rij = glm::length(chargedParticles[j].oldPosition - chargedParticles[indx].oldPosition);
				glm::vec2 divater = glm::vec2(0.5)*(chargedParticles[j].oldPosition + chargedParticles[indx].oldPosition) - cm[0].oldPosition;
				rij /= (glm::length(divater) + epsilon);
				if (rij < a)
					F += glm::vec2(chargedParticles[j].q*rij*p / (a*a*a))*(chargedParticles[j].oldPosition - chargedParticles[indx].oldPosition);
				else
					F+=glm::vec2(chargedParticles[j].q*p/(rij*rij))*(chargedParticles[j].oldPosition - chargedParticles[indx].oldPosition);
			}
		}
		return F;
	}

	void updatePositionAndVeliocity(int indx)
	{
		glm::vec2 F = determineTheImpact(indx);
		chargedParticles[indx].position = chargedParticles[indx].oldPosition + random2(engine)*glm::vec2(ka)*F + random2(engine)*glm::vec2(kv)*chargedParticles[indx].veloicity;
		chargedParticles[indx].veloicity = chargedParticles[indx].position - chargedParticles[indx].oldPosition;
	}

	void correctPosition(int indx)
	{
		glm::vec2 newPos = random(engine);
		if (chargedParticles[indx].position.x<v1.x || chargedParticles[indx].position.x>v2.x)
			chargedParticles[indx].position.x = newPos.x;
		if (chargedParticles[indx].position.y<v1.y || chargedParticles[indx].position.y>v2.y)
			chargedParticles[indx].position.y = newPos.y;
	}

	void actualizeCm()
	{
		int j = 0;
		for (int i = 0; i < cm.size();++i)
		{
			for (; j < cm.size(); ++j)
			{
				if (chargedParticles[i].f < cm[j].f)
				{
					cm[j] = chargedParticles[i];
					break;
				}

			}
		}
	}

};
//#endif // !CHARGED_SYSTEM_SEARCH_H