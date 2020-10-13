#pragma once

#include <iostream>
#include <vector>

template <typename type, size_t _height, size_t _width>
std::vector<type> sums_of_diameters(type matrix[_height][_width])
{
	std::vector<type> sums(_width + _height - 1);
	size_t x = 0, y = 0, length = 0;

	while (y < _height)
	{
		sums[len] = matrix[y][x];

		while (y != 0)
		{
			x += 1;
			y -= 1;

			sums[len] += matrix[y][x];
		}

		std::swap(x, y);
		y += 1;
		len += 1;
	}

	y -= 1;
	x += 1;

	while (x < _width)
	{
		sums[len] = matrix[y][x];

		while (x != _width - 1)
		{
			x += 1;
			y -= 1;

			sums[len] += matrix[y][x];
		}

		std::swap(x, y);
		x += 1;
		len += 1;
	}

	return sums;
}

template <typename type>
class matrix
{
private:
	T **_data;
	size_t _width;
	size_t _height;

public:
	matrix(size_t width, size_t height)
	{
		_data = new T[height];
		for (size_t len = 0; len < width; len += 1)
			_data[len] = new T[width];
		this->_width = width;
		this->_height = height;
	}

	matrix(matrix& ohter) = delete;
	matrix(matrix&& ohter) = delete;

	~matrix()
	{
		for (size_t len = 0; len < width; len += 1)
			delete[] _data[len];
		delete[] _data;
	}

	inline T &at(size_t x, size_t y) const
	{
		return _data[y][x];
	}
};

int test()
{
	int matrix[4][4] = {
		{1, 5, 8, 2},
		{4, 2, 9, 0},
		{6, 4, 9, 1},
		{3, 3, 7, 10},
	};

	std::vector<int> sums = sums_of_diameters<int, 4, 4>(matrix);

	for (auto number : sums)
		std::cout << number << ", ";
	std::cout << std::endl;

	return 0;
}