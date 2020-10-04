#pragma once

#include <cstdio>

#include "io.h"
#include "types.h"
#include "cleaner.h"

namespace counter
{
	using namespace scl;

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
		constexpr size_t buffer_size = 4096;
		ubuffer_t buffer;

		buffer.pntr = (ubyte *)mem::safe_malloc(buffer_size);
		if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return;
		}
		cleaner::add_free(buffer.pntr);
		

	}
}