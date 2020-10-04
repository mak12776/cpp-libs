#pragma once

#include <cstdio>

#include "io.h"
#include "types.h"

namespace counter
{
	struct count_t
	{
		size_t chars;
		size_t lines;
		size_t numbers;
		size_t words;
	};

	// there is two types of file readers.
	// buffered readers & full buffered readers

	static inline void count_file(FILE *file, count_t &count)
	{
		using namespace scl;
		ubuffer_t buffer;

		// io::fread_all(file, buffer.pntr, buffer.size);
	}
}