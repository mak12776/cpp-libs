// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include "scl.h"

using namespace scl::pxld;
using namespace scl::error;

color_t func(point_t point)
{
	return 0x888888;
}

int main()
{
	image_t image;
	point_t size = { 1000, 1000 };

	image::initialize(&image, size, mode::RGB);
	check_error();

	image::draw(&image, func);
	check_error();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

