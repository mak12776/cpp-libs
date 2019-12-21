// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

typedef uint64_t number_t;
#define NUMBER_MAX UINT64_MAX

static inline void write_primes(ostream &stream, number_t max)
{
	std::vector<number_t> numbers = { 2, 3 };

	number_t n = numbers.back() + 2;

	for (auto it = numbers.cbegin(); it != numbers.cend(); it++)
	{
		if (n % *it == 0)
		{
			n = numbers.back() + 2;

			it = numbers.cbegin() + 2;
		}
	}

	for (auto it = numbers.cbegin(); it != numbers.cend(); it++)
	{
		stream << *it << endl;
	}
}

int math_main()
{
	ofstream stream("primes.txt", std::ios::binary);

	write_primes(std::cout, NUMBER_MAX);
	return 0;
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

