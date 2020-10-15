#pragma once

#include <cstdio>

#include "scl/all.h"

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

	template <size_t buffer_size = 4096>
	static inline void count_file(FILE *file, count_t &count)
	{
		ubuffer_t buffer;

		io::fread_all(file, buffer);

		if (err::clear_if(err::NO_MEMORY))
		{
			buffer.pntr = mem::safe_malloc(buffer_size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			cleaner::add_free(buffer.pntr);

			// more codes here
		}
		else if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return;
		}
	}
}