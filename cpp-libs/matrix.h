#pragma once

#include <iostream>
#include <vector>

template <typename type, size_t height, size_t width>
std::vector<type> sums_of_diameters(type matrix[height][width])
{
	std::vector<type> sums(width + height - 1);
	size_t x = 0, y = 0, index = 0;

	while (y < height)
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

	while (x < width)
	{
		sums[index] = matrix[y][x];

		while (x != width - 1)
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