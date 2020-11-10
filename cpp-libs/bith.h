#pragma once

#include "scl/all.h"

namespace bith
{
	typedef bool(&size_manager_t)(size_t &);

	bool double_size_manager(size_t &size)
	{
		if (size == 0)
			size = 1024;
		scl::math::safe_mul(size, (size_t)2, size);
		return scl::err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
	}

	struct segment_buffer_t
	{
		size_t data_bits;
		size_t data_size;

		struct
		{
			size_t len;
			size_t size;

			scl::ubyte *data_pntr;
			size_t *counts_pntr;
		} data_counts;

		struct
		{
			scl::ubyte *pntr;
			size_t bits;
			size_t size;
		} remaining;
	};


	template <typename data_type, size_manager_t size_manager>
	static inline void count_primary_types(scl::ubuffer_t &buffer, segment_buffer_t &result)
	{
		data_type *pntr = buffer.pntr;
		data_type *end = buffer.pntr + (buffer.size / sizeof(data_type));

		
	}

	template <size_manager_t size_manager = double_size_manager>
	static inline void count_bits(size_t data_bits, scl::ubuffer_t &buffer, segment_buffer_t &result)
	{
		size_t buffer_bits;

		math::safe_mul(buffer.size, (size_t)8, buffer_bits);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		result.data_bits = data_bits;
		result.data_size = (data_bits / 8) + (data_bits % 8) ? 1 : 0;

		size_t possible_data_number;

		math::safe_pow((size_t)2, data_bits, possible_data_number);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		printf("possible data number: %zu\n", possible_data_number);
	}

	static inline void count_bits(size_t data_bits, const char *name, segment_buffer_t &result)
	{
		namespace err = scl::err;

		scl::ubuffer_t buffer;

		scl::io::fopen_fread_malloc(name, buffer);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		count_bits(data_bits, buffer, result);
		err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);

		free(buffer.pntr);
	}
}