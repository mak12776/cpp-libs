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

	struct segmented_buffer_t
	{
		struct
		{
			size_t buffer_bits;
			size_t buffer_size;

			size_t bits;
			size_t size;

			size_t possible_data_number;
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

		template <size_manager_t size_manager>
		inline void calculate_primitive_size_bits()
		{
			// get default size_manager value
			size_manager(this->counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// choose a minimum valuue for counts size
			this->counts.size = std::min<size_t>(this->info.possible_data_number, this->counts.size);
			this->counts.len = 0;

			// calculating remaining bits & size
			this->remaining.bits = this->info.buffer_bits % this->info.bits;
			this->remaining.size = get_bytes_per_bits(this->remaining.bits);
		}

		template <typename data_type>
		inline void malloc_primitive_types()
		{
			// allocate memory for remaining
			this->remaining.pntr = (ubyte *)mem::safe_malloc(this->remaining.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// allocate memory for data pntr
			this->counts.data_pntr = (ubyte *)mem::safe_malloc_array<data_type>(this->counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// allocate memory for counts pntr
			this->counts.counts_pntr = mem::safe_malloc_array<size_t>(this->counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;
		}

	};

	template <typename data_type, size_manager_t size_manager>
	static inline void count_primitive_types(ubuffer_t &buffer, segmented_buffer_t &result)
	{
		data_type *pntr;
		data_type *end;

		result.calculate_primitive_size_bits<size_manager>();
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		result.malloc_primitive_types<data_type>();
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		// --- main process ---
		pntr = (data_type *)buffer.pntr;
		end = (data_type *)(buffer.pntr + buffer.size - (buffer.size % result.info.size));

		while (pntr <= end)
		{

			pntr += 1;
		}

		printf("success.\n");
	}


	template <size_manager_t size_manager = double_size_manager>
	static inline void count_bits(size_t data_bits, ubuffer_t &buffer, segmented_buffer_t &result)
	{
		// calculating buffer informations
		result.info.buffer_size = buffer.size;
		math::safe_mul(buffer.size, (size_t)8, result.info.buffer_bits);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__)) return;

		// calculating data informations
		result.info.bits = data_bits;
		result.info.size = get_bytes_per_bits(data_bits);

		// calculating possible data number
		math::safe_pow<size_t>((size_t)2, result.info.bits, result.info.possible_data_number);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__)) return;

		// --- [main process] ---
		if (result.info.bits % 8 == 0)
		{
			if (result.info.size == sizeof(uint8_t))
			{
				count_primitive_types<uint8_t, size_manager>(buffer, result);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}
			else if (result.info.size == sizeof(uint16_t))
			{
				count_primitive_types<uint16_t, size_manager>(buffer, result);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}
			else if (result.info.size == sizeof(uint32_t))
			{
				count_primitive_types<uint32_t, size_manager>(buffer, result);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}
			else if (result.info.size == sizeof(uint64_t))
			{
				count_primitive_types<uint64_t, size_manager>(buffer, result);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}

			// needs more code
		}

		err::set(err::INVALID_ARGUMENT);
		err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
	}

	static inline void count_bits(size_t data_bits, const char *name, segmented_buffer_t &result)
	{
		scl::ubuffer_t buffer;

		scl::io::fopen_malloc_fread(name, (void **)&buffer.pntr, &buffer.size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		count_bits(data_bits, buffer, result);
		err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);

		free(buffer.pntr);
	}
}