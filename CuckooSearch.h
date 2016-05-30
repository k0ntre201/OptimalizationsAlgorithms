#ifndef CUCKOO_SEARCH_H
#define CUCKOO_SEARCH_H

#include <glm\glm.hpp>
#include <functional>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <memory>
#include <iostream>
#include <thread>
#include "MyRandom.h"
#include "DrawingClass.h"

class CuckooSearch
{
	struct Cuckoo
	{
		glm::vec2 position;
		double f;
	};
public:
	CuckooSearch(std::function<double(double, double)> fun, const glm::vec2& v1, const glm::vec2& v2, int N = 25, double pa = 0.25)
	{
		f = fun;
		this->v1 = v1;
		this->v2 = v2;
		this->pa = pa;
		engine = std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		vd = TwoDimensionUniformRandom(v1, v2);
		uid = std::uniform_int_distribution<int>(0, N - 1);
		cuckoos.resize(N);
		u.resize(N);
		ran = std::uniform_real_distribution<double>(0.0, 1.0);
		drawingClass.reset(new DrawingClass(f, v1, v2));
	}

	void run(int maxIterations = 50)
	{
		initialize();
		SDL_Event e;
		bool quit = false;
		int t = 0;
		bool end;
		while (!quit)
		{
			if (SDL_PollEvent(&e))
				if (e.type == SDL_QUIT)
					quit = true;



			std::cout << t << "\n";
			std::cout << bestCuckoo.position.x << " " << bestCuckoo.position.y << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			if (t < maxIterations)
			{
				evalutaEFunctionsValue();
				for (auto& cuckoo : cuckoos)
				{
					actualizePositionWithLevyFlight(cuckoo);
					actualizeNest(cuckoo);
				}
				Cuckoo temp = getBestCucko();
				if (temp.f < bestCuckoo.f)
					bestCuckoo = temp;
				toDraw();
				++t;
			}
			drawingClass->drawPoinstOnFunction(u);
		}
	}

private:
	void initialize()
	{
		for (int i = 0; i < cuckoos.size(); ++i)
		{
			cuckoos[i].position = vd(engine);
			cuckoos[i].f = f(cuckoos[i].position.x, cuckoos[i].position.y);
		}
		bestCuckoo = getBestCucko();
	}

	void evalutaEFunctionsValue()
	{
		for (int i = 0; i < cuckoos.size(); ++i)
		{
			cuckoos[i].f = f(cuckoos[i].position.x, cuckoos[i].position.y);
		}
	}

	void actualizePositionWithLevyFlight(Cuckoo& cuckoo)
	{
		Cuckoo newCuckoo;
		newCuckoo.position.x = alpha*levy(engine);
		newCuckoo.position.y = alpha*levy(engine);
		newCuckoo.position *= glm::vec2(0.01)*(cuckoo.position - bestCuckoo.position);
		newCuckoo.position += cuckoo.position;
		boundsLimit(cuckoo);
		newCuckoo.f = f(newCuckoo.position.x, newCuckoo.position.y);
		if (newCuckoo.f < cuckoo.f)
			cuckoo = newCuckoo;
	}

	void actualizeNest(Cuckoo& cuckoo)
	{
		glm::vec2 stepSize = (cuckoos[uid(engine)].position - cuckoos[uid(engine)].position);
		double k = 0;
		if ((k = ran(engine)) <= pa)
			k = 0;
		stepSize *= glm::vec2(k);
		Cuckoo c;
		c.position= cuckoo.position + glm::vec2(ran(engine))*stepSize;
		boundsLimit(c);
		c.f = f(c.position.x, c.position.y);
		if (c.f < cuckoo.f)
			cuckoo = c;
	}

	Cuckoo getBestCucko()
	{
		auto it = std::min_element(std::begin(cuckoos), std::end(cuckoos), [](Cuckoo& c1, Cuckoo& c2) { return c1.f < c2.f; });
		return *it;
	}

	void boundsLimit(Cuckoo& cuckoo)
	{
		
		glm::vec2 temp = vd(engine);
		if (cuckoo.position.x < v1.x || cuckoo.position.x > v2.x || cuckoo.position.y < v1.y || cuckoo.position.y > v2.y)
			cuckoo.position = temp;
	}

	void toDraw()
	{
		for (int i = 0; i < cuckoos.size(); ++i)
			u[i] = cuckoos[i].position;
	}

private:
	std::function<double(double, double)> f;
	glm::vec2 v1, v2;
	std::valarray<Cuckoo> cuckoos;
	Cuckoo bestCuckoo;
	double beta = 1.5,alpha = 1.0,pa;
	std::unique_ptr<DrawingClass> drawingClass;
	std::valarray<glm::vec2> u;
	std::uniform_int_distribution<int> uid;
	MyRandom::inverse_gaussian_distribution<double> levy;
	std::default_random_engine engine;
	TwoDimensionUniformRandom vd;
	std::uniform_real_distribution<double> ran;
};
#endif // !CUCKOO_SEARCH_H
