#pragma once

#include "../scl/all.h"

namespace bith
{
	using namespace scl;

	typedef size_t(&realloc_more_t)(size_t);
	
	struct segment_buffer_t
	{
		size_t size;
		size_t len;
		size_t *count_array;

		struct
		{
			size_t bits;
			size_t size;
			ubyte *buffer;
		} buffer;

		struct
		{
			size_t bits;
			size_t size;
			ubyte *buffer;
		} remaining;
	};

	static inline void count_bits(size_t data_bits, ubuffer_t &buffer, segment_buffer_t &result)
	{
		size_t buffer_bits;
		size_t data_size;

		math::safe_mul(buffer.size, (size_t)8, buffer_bits);
		data_size = (data_bits / 8) + (data_bits % 8) ? 1 : 0;


	}
}