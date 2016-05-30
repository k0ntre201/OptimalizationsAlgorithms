#ifndef MATRIX_H
#define MATRIX_H

#include <valarray>

template<typename T>
class Matrix
{
public:
	Matrix() {}
	Matrix(int c, int r)
	{
		row = r;
		collumn = c;
		data.resize(r*c);
	}

	T& at(int i, int j)
	{
		return data[i*collumn + j];
	}

	T max()
	{
		return data.max();
	}

	T min()
	{
		return data.min();
	}

	template<typename F>
	void apply(F f)
	{
		data.apply(f);
	}

	int getRow()
	{
		return row;
	}

	int getCollumn()
	{
		return collumn;
	}
private:
	std::valarray<T> data;
	int row, collumn;
};

#endif // !MATRIX_H

