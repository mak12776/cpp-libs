#pragma once

#include <iostream>
#include <vector>

template <typename type, size_t _height, size_t _width>
std::vector<type> sums_of_diameters(type matrix[_height][_width])
{
	std::vector<type> sums(_width + _height - 1);
	size_t x = 0, y = 0, index = 0;

	while (y < _height)
	{
		sums[index] = matrix[y][x];

		while (y != 0)
		{
			x += 1;
			y -= 1;

			sums[index] += matrix[y][x];
		}

		std::swap(x, y);
		y += 1;
		index += 1;
	}

	y -= 1;
	x += 1;

	while (x < _width)
	{
		sums[index] = matrix[y][x];

		while (x != _width - 1)
		{
			x += 1;
			y -= 1;

			sums[index] += matrix[y][x];
		}

		std::swap(x, y);
		x += 1;
		index += 1;
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
		for (size_t index = 0; index < width; index += 1)
			_data[index] = new T[width];
		this->_width = width;
		this->_height = height;
	}

	matrix(matrix& ohter) = delete;
	matrix(matrix&& ohter) = delete;

	~matrix()
	{
		for (size_t index = 0; index < width; index += 1)
			delete[] _data[index];
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