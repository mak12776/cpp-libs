#pragma once

#include "scl/all.h"

namespace bith
{
	using namespace scl;

	typedef void(&size_manager_t)(size_t &);

	void double_size_manager(size_t &size)
	{
		if (size == 0)
			size = 1024;
		math::safe_mul(size, (size_t)2, size);
		err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
	}

	static constexpr inline size_t get_bytes_per_bits(size_t bits)
	{
		return (bits / 8) + ((bits % 8) ? 1 : 0);
	}

	struct segment_buffer_t
	{
		struct
		{
			size_t buffer_bits;
			size_t buffer_size;

			size_t bits;
			size_t size;
		} info;

		struct
		{
			size_t size;
			size_t len;

			ubyte *data_pntr;
			size_t *counts_pntr;
		} counts;

		struct
		{
			size_t bits;
			size_t size;
			ubyte *pntr;
		} remaining;
	};

	template <typename data_type, size_manager_t size_manager>
	static inline void count_primitive_types(ubuffer_t &buffer, segment_buffer_t &result)
	{
		size_t possible_data_number;
		size_t cleaner_start_index;

		data_type *pntr;
		data_type *end;

		// --- calculating informations ---

		// calculating possible data number
		math::safe_pow<size_t>((size_t)2, sizeof(data_type) * 8, possible_data_number);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__)) return;

		// calculating remaining bits & size
		result.remaining.bits = buffer_bits % result.info.bits;
		result.remaining.size = get_bytes_per_bits(result.remaining.bits);

		// choose a minimum allocation size
		size_manager(result.counts.size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__)) return;
		result.counts.size = std::min<size_t>(possible_data_number, result.counts.size);
		result.counts.len = 0;

		// calculating data bytes
		math::safe_mul(result.counts.size, result.info.size, data_bytes);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__)) return;

		// ---- memory allocations --

		// get cleaner start index
		cleaner_start_index = cleaner::get_index();

		// allocate memory for remaining
		result.remaining.pntr = mem::safe_malloc(result.remaining.size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__)) return;
		cleaner::add_free(result.remaining.pntr); // [from now we need call cleaner::finish at end]
		
		// allocate memory for data pntr
		result.counts.data_pntr = mem::safe_malloc(data_bytes);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return cleaner::finish(cleaner_start_index);
		cleaner::add_free(result.counts.data_pntr);

		// allocate memory for counts pntr
		result.counts.counts_pntr = mem::safe_malloc_array<size_t>(result.info.size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
		{
			cleaner::finish(cleaner_start_index);
			return;
		}
		cleaner::add_free(result.counts.counts_pntr);

		pntr = buffer.pntr;
		end = buffer.pntr + (buffer.size / sizeof(data_type));
	}


	template <size_manager_t size_manager = double_size_manager>
	static inline void count_bits(size_t data_bits, ubuffer_t &buffer, segment_buffer_t &result)
	{
		// calculating data informations
		result.info.buffer_size = buffer.size;
		result.info.buffer_bits = buffer.size * 8;

		result.info.bits = data_bits;
		result.info.size = get_bytes_per_bits(data_bits);

		if (result.info.bits % 8 == 0)
		{
			if (result.info.size == sizeof(uint8_t))
			{
				count_primitive_types<uint8_t>(buffer, result);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}
			else if (result.info.size == sizeof(uint16_t))
			{
				count_primitive_types<uint16_t>(buffer, result);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}
			else if (result.info.size == sizeof(uint32_t))
			{
				count_primitive_types<uint32_t>(buffer, result);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}
			else if (result.info.size == sizeof(uint64_t))
			{
				count_primitive_types<uint64_t>(buffer, result);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}


		}
	}

	static inline void count_bits(size_t data_bits, const char *name, segment_buffer_t &result)
	{
		scl::ubuffer_t buffer;

		scl::io::fopen_fread_malloc(name, buffer);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		count_bits(data_bits, buffer, result);
		err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);

		free(buffer.pntr);
	}
}