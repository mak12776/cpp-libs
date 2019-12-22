// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define _CRT_SECURE_NO_WARNINGS

#include "Person.h"

static inline void print_sep(string sep, string end = "\n", uint64_t repeat = 80)
{
	for (uint64_t step = 0; step < repeat; step += 1)
		cout << std::string(sep);
	cout << std::string(end);
}

int main()
{
	Person me{"me", "Amin", 20 };
	Person you = Person("you", "Armin", 30);
	Person another("another", "ali", 40);

	print_sep("-");

	you = me;

	print_sep("-");
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

