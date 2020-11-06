#pragma once

#include "../scl/all.h"

namespace bith
{
	using namespace scl;

	typedef bool(&size_manager_t)(size_t &);

	bool double_size_manager(size_t &size)
	{
		if (size == 0)
			size = 1024;
		math::safe_mul(size, (size_t)2, size);
		return err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
	}

	struct segment_buffer_t
	{
		size_t data_count_size;
		size_t data_count_len;
		size_t *count_array;

		struct
		{
			size_t bits;
			size_t size;
			struct
			{
				ubyte *pntr;
				size_t size;
			} buffer;
		} data;

		struct
		{
			size_t bits;
			size_t size;
			ubyte *pntr;
		} remaining;
	};


	template <typename data_type, size_manager_t size_manager>
	static inline void count_primary_types(ubuffer_t &buffer, segment_buffer_t &result)
	{
		data_type *pntr = buffer.pntr;
		data_type *end = buffer.pntr + (buffer.size / sizeof(data_type));

		
	}

	template <size_manager_t size_manager = double_size_manager>
	static inline void count_bits(size_t data_bits, ubuffer_t &buffer, segment_buffer_t &result)
	{
		size_t buffer_bits;

		math::safe_mul(buffer.size, (size_t)8, buffer_bits);

		result.data.bits = data_bits;
		result.data.size = (data_bits / 8) + (data_bits % 8) ? 1 : 0;

		switch (result.data.bits)
		{
		case sizeof(uint8_t):
		}

		result.data_count_size = 0;
		if (size_manager(result.data_count_size))
			return;
		

	}
}