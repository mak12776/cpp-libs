// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

constexpr size_t buffer_size = 1024 * 8;
const uint8_t buffer_ptr[buffer_size] = { 0 };

int main(int argc, char *argv[])
{
	const char *program_name = (argc == 0) ? "count" : argv[0];

	if (argc != 2)
	{
		std::cout << "usage: " << program_name << " [FILE]" << std::endl;
		return EXIT_SUCCESS;
	}

	scl::buffer<8192> buff;
	std::ifstream stream(argv[1], std::ios::binary);
	size_t total;

	total = 0;
	while (!stream.eof())
	{
		try
		{
			stream.read(buff.data(), buff.size());
			for (std::streamsize index = 0; index < stream.gcount(); index += 1)
				if (buff[index] == '\n')
					total += 1;
		} 
		catch (std::ios::failure &e)
		{
			std::cout << "can't read file: " << e.what() << std::endl;
			return EXIT_FAILURE;
		}
	}

	std::cout << "total line: " << total << std::endl;
	return EXIT_SUCCESS;
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

