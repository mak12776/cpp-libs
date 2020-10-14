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

	template <size_t buffer_size = 4096, default_scl_t &ncl = default_scl>
	static inline void count_file(FILE *file, count_t &count)
	{
		ubuffer_t buffer;

		io::fread_all(file, buffer);
		if (ncl.err.check())
		{
			if (ncl.err.test_clear(err::MALLOC))
			{
				buffer.pntr = ncl.mem.safe_malloc(buffer_size);
				if (ncl.err.check())
				{
					ncl.err.push_file_info(__FILE__, __LINE__, __FUNCTION__);
					return;
				}
				ncl.cleaner.add_free(buffer.pntr);

				// more codes there
			}

			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return;
		}


	}
}