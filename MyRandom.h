#ifndef MY_RANDOM_H
#define MY_RANDOM_H

#include <glm\glm.hpp>
#include <random>
#include <cmath>
#include <type_traits>


class TwoDimensionUniformRandom
{
public:
	TwoDimensionUniformRandom() {}
	TwoDimensionUniformRandom(glm::vec2 v1, glm::vec2 v2)
	{
		dx = std::uniform_real_distribution<float>(v1.x, v2.x);
		dy = std::uniform_real_distribution<float>(v1.y, v2.y);
	}
	~TwoDimensionUniformRandom() {}
	template<typename Engine>
	glm::vec2 operator()(Engine& engine)
	{
		return glm::vec2(dx(engine), dy(engine));
	}
private:
	std::uniform_real_distribution<float> dx, dy;
};

class TwoIntegerDimensionUniformRandom
{
public:
	TwoIntegerDimensionUniformRandom() {}
	TwoIntegerDimensionUniformRandom(glm::ivec2 v1, glm::ivec2 v2)
	{
		dx = std::uniform_int_distribution<int>(v1.x, v2.x);
		dy = std::uniform_int_distribution<int>(v1.y, v2.y);
	}
	~TwoIntegerDimensionUniformRandom() {}
	template<typename Engine>
	glm::ivec2 operator()(Engine& engine)
	{
		return glm::ivec2(dx(engine), dy(engine));
	}
private:
	std::uniform_int_distribution<int> dx, dy;
};

namespace MyRandom
{
	//linear generator using Park, Miller and Carta paramerts
	class basic_linear_generator
	{
	public:
		basic_linear_generator(unsigned long long seed = 28650,
			unsigned long long a = 16807,
			unsigned long long c = 0,
			unsigned long long m = std::pow(2, 32) - 1);
		long long operator()();
		virtual ~basic_linear_generator();
	private:
		unsigned long long a, c, m;
		unsigned long long seed;
	};

	inline basic_linear_generator::basic_linear_generator(unsigned long long seed,
		unsigned  long long a,
		unsigned long long c,
		unsigned long long m)
	{
		this->seed = seed;
		this->a = a;
		this->c = c;
		this->m = m;
	}

	inline long long basic_linear_generator::operator()()
	{
		return (seed = (a*seed + c) % m);
	}

	inline basic_linear_generator::~basic_linear_generator()
	{
	}

	class modified_basic_linear_generator
	{
	public:
		modified_basic_linear_generator(unsigned int seed)
		{
			seed1 = seed;
			mixSeed();
		}
		unsigned int operator()()
		{
			seed3 = (1176 * seed3 + 1476 * seed2 + 1776 * seed1) % (unsigned int)(std::pow(2, 32) - 5);
			seed1 = seed2;
			seed2 = seed3;
			return seed3;
		}
		~modified_basic_linear_generator() {}
	private:
		unsigned int seed1, seed2, seed3;
		void mixSeed()
		{
			seed2 = (seed1 & 0x00fff45) >> 2;
			seed2 = (seed2 & 0x5621ab) << 1;
			seed3 = ((seed1^seed2) << 3) & 0x85ffac9;
		}
	};

	class tausworthe_generator
	{
	public:
		tausworthe_generator(unsigned int seed)
		{
			s1 = seed;
			s2 = (s1 & 0x00fff45) >> 2;
			s2 = (s2 & 0x5621ab) << 1;
			s3 = ((s1^s2) << 3) & 0x85ffac9;
		}
		unsigned int operator()()
		{
			unsigned int b1 = ((s1 << 9) ^ s1) << 4;
			s1 = ((s1 << 12) ^ (b1 >> 20));
			return s1;// ^s2^s3;
		}
		~tausworthe_generator() {}
	private:
		unsigned int s1, s2, s3;
	};


	/*
	*distributions
	*/
	//uniform distribution for integer numbers

	template<typename T>
	class uniform_integer_distribution
	{
	public:
		static_assert(std::is_integral<T>::value, "it isn't integral type!");
		uniform_integer_distribution(T a, T b)
		{
			this->a = a;
			this->b = b;
		}
		template<typename Engine>
		T operator()(Engine& engine)
		{
			return engine() % (b - a) + a;
		}
		virtual ~uniform_integer_distribution() {}
	private:
		T a, b;

	};

	template<typename T>
	class uniform_real_distribution
	{
	public:
		static_assert(std::is_floating_point<T>::value, "It isn't a floating point type!\n");
		uniform_real_distribution(T a, T b)
		{
			this->a = a;
			this->b = b;
		}
		template<typename Engine>
		T operator()(Engine& engine)
		{
			return engine()*2.3283064365e-10 * (b - a) + a;
		}
		~uniform_real_distribution() {}

	private:
		T a, b;
	};

	template<typename T>
	class cauchy_distribution
	{
	public:
		cauchy_distribution(T a = 0)
		{
			offsed = a;
		}
		template<typename Engine>
		T operator()(Engine& engine)
		{
			MyRandom::uniform_real_distribution<T> dist(0.0, 1.0);
			/*T x{ 0 }, u{ 0 };
			while (true)
			{

			x = dist(engine);
			u = dist(engine);
			if (0.5*u <= f(x) - fm12)
			break;
			}
			return x + offsed;*/

			return std::tan(3.1415926535897*((dist(engine) - 0.5))) + offsed;

		}
		virtual ~cauchy_distribution() {}
	private:
		T offsed;
		const double fm = 2.0 / 3.1415926535897;
		const double fm12 = fm - 0.5;
		T f(T x)
		{
			return  fm / (1 + x*x);
		}
	};

	template<typename T>
	class box_muller_transform
	{
	public:
		box_muller_transform(T m = 1, T o = 0)
		{
			mean = m;
			offsed = o;
		}

		template<typename Engine>
		T operator()(Engine& engine)
		{
			MyRandom::uniform_real_distribution<T> dist(0.0, 1.0);

			T u1 = dist(engine);
			T u2 = dist(engine);


			T z0 = std::sqrt(-2 * std::log(u1))*std::cos(2 * 3.1415926535897*u2);//  std::sqrt(-2 * std:log(u1))*std::cos(2 * 3.1415926535897*u2);
																				 //T z1 = std::sqrt(-2 * std:log(u1))*std::sin(2 * 3.1415926535897*u2);

			return z0*mean + offsed;
		}

		virtual ~box_muller_transform() {}
	private:
		T mean, offsed;
	};

	template<typename T>
	class mashagilla_polar_method
	{
	public:
		mashagilla_polar_method(T mean = 0, T sigma = 1)
		{
			this->mean = mean;
			this->sigma = sigma;
		}

		template<typename Engine>
		T operator()(Engine& engine)
		{
			MyRandom::uniform_real_distribution<T> dist(0.0, 1.0);
			T u, v, s;
			while (true)
			{
				u = dist(engine);
				v = dist(engine);
				if ((s = u*u + v*v) < 1)
					break;
			}
			return (u*std::sqrt(-std::log(s) / s))*sigma + mean;
		}

		virtual ~mashagilla_polar_method() {}
	private:
		T mean, sigma;
	};

	template<typename T>
	class inverse_gaussian_distribution
	{
	public:
		inverse_gaussian_distribution(T mean = 1, T shapness = 1)
		{
			ni = mean;
			lambda = shapness;
		}
		template<typename Engine>
		T operator()(Engine& engine)
		{
			MyRandom::uniform_real_distribution<T> dist(0.0, 1.0);
			MyRandom::mashagilla_polar_method<T> N;
			T u = N(engine);
			T y = u*u;

			T x = ni + ((ni*ni*y) - ni*(std::sqrt(4 * ni*lambda*y + ni*ni*y*y))) / (2 * lambda);

			T z = dist(engine);

			if (z <= (ni / (ni + x)))
				return x;
			else
				return (ni*ni) / x;
		}

		virtual ~inverse_gaussian_distribution() {}

	private:
		T ni, lambda;

	};

	template<typename T>
	class levy_distribution
	{
	public:
		static_assert(std::is_fundamental<T>::value, "Wrong intial parametrs in template!\n");

		levy_distribution(T mean = 1, T shapness = 1)
		{
			ni = mean;
			lambda = shapness;
		}
		template<typename Engine>
		T operator()(Engine& engine)
		{
			MyRandom::uniform_real_distribution<T> dist(-0.5, 0.5);
			MyRandom::mashagilla_polar_method<T> N;
			T u = N(engine);
			T y = u*u;

			T x = ni + ((ni*ni*y) - ni*(std::sqrt(4 * ni*lambda*y + ni*ni*y*y))) / (2 * lambda);

			T z = dist(engine);

			T ld;
			if (z <= (ni / (ni + x)))
				ld = x;
			else
				ld = (ni*ni) / x;

			return lambda / (ld*ld) + ni;
		}

		virtual ~levy_distribution() {}

	private:
		T ni, lambda;
	};
}


#endif // !MY_RANDOM_H