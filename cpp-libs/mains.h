#pragma once

#include <iostream>
#include <fstream>
#include "scl.h"

namespace mains
{
	constexpr size_t buffer_size = 1024 * 8;
	const uint8_t buffer_ptr[buffer_size] = { 0 };

	int count(int argc, char *argv[])
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
}