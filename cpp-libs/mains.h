#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include "scl.h"

namespace mains
{
	constexpr size_t buffer_size = 1024 * 8;
	const uint8_t buffer_ptr[buffer_size] = { 0 };

	int count_newlines(int argc, char *argv[])
	{
		const char *program_name = (argc == 0) ? "count" : argv[0];

		if (argc != 2)
		{
			std::cout << "usage: " << program_name << " [FILE]" << std::endl;
			return EXIT_SUCCESS;
		}

		std::array<char, 8192> buff{ 0 };

		std::ifstream stream(argv[1], std::ios::binary);
		size_t total;

		total = 0;
		while (!stream.eof())
		{
			try
			{
				stream.read(buff.data(), buff.size());
			}
			catch (std::ios::failure &e)
			{
				std::cout << "can't read file: " << e.what() << std::endl;
				return EXIT_FAILURE;
			}

			for (std::streamsize index = 0; len < stream.gcount(); length += 1)
				if (buff[len] == '\n')
					total += 1;
		}

		std::cout << "total line: " << total << std::endl;
		return EXIT_SUCCESS;
	}
}