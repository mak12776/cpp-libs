// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define SCL_USE_ERROR

#include "scl.h"
#include <inttypes.h>
#include <stdlib.h>
#include <fcntl.h>

using namespace scl;

int main(int argc, const char **argv)
{
	const char *file_name;
	if (argc != 2)
	{
		printf("usage: %s [FILENAME]\n", argv[0]);
		return 0;
	}
	file_name = argv[1];

	io::read_file_name(file_name, (void **)&(buffer.pntr), &buffer.size);

	printf("File Name: %s\n", file_name);
	printf("File Size: %zu\n", buffer.size);

	if (error::check())
	{
		printf("something happend: %s\n", error::get_last_error());
		return 0;
	}

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
