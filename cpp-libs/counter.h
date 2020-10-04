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

	using scl::err::default_handler_t;
	using scl::err::default_handler;

	using scl::mem::manager_t;
	using scl::mem::default_manager;

	using scl::cleaner::default_cleaner_t;
	using scl::cleaner::default_cleaner;

	template <
		default_handler_t &handler = default_handler,
		manager_t<handler> &manager = default_manager,
		default_cleaner_t &cleaner = default_cleaner>
	static inline void count_file(FILE *file, count_t &count)
	{
		ubuffer_t buffer;

		buffer.pntr = manager.safe_malloc(buffer_size);
		if (handler.check())
		{
			handler.push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return;
		}
		cleaner.add_free(buffer.pntr);
		

	}
}