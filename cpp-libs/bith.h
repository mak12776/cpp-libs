#pragma once

#include "scl/all.h"

namespace bith
{
	using namespace scl;

	typedef void(&size_manager_t)(size_t &);

	void half_size_manager(size_t &size)
	{
		if (size < 1024)
		{
			size = 1024;
			return;
		}

		math::safe_add(size, size / 2, size);
		err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
	}

	void double_size_manager(size_t &size)
	{
		if (size < 1024)
		{
			size = 1024;
			return;
		}
		math::safe_mul(size, (size_t)2, size);
		err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
	}

	void adder_size_manager(size_t &size)
	{
		if (size == 0)
		{
			size = 1024;
			return;
		}
		math::safe_add(size, (size_t)1024, size);
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

			inline void calculate(size_t data_bits, size_t buffer_size)
			{
				// calculating buffer informations
				this->buffer_size = buffer_size;

				math::safe_mul(buffer_size, (size_t)8, this->buffer_bits);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;

				// calculating data informations
				this->bits = data_bits;
				this->size = get_bytes_per_bits(data_bits);

				// calculating possible data number
				math::safe_pow<size_t>((size_t)2, data_bits, this->possible_data_number);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}
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

		// --- primitive types ---

		template <typename data_type, size_manager_t size_manager>
		inline void malloc_primitive_types()
		{
			// --- calculate size & bits ---
			size_manager(counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			counts.size = std::min<size_t>(info.possible_data_number, counts.size);
			counts.len = 0;

			// remaining size & bits
			remaining.bits = info.buffer_bits % info.bits;
			remaining.size = get_bytes_per_bits(remaining.bits);

			// --- allocate memory ---
			remaining.pntr = (ubyte *)mem::safe_malloc(remaining.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			counts.data_pntr = (ubyte *)mem::safe_malloc_array<data_type>(counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			counts.counts_pntr = mem::safe_malloc_array<size_t>(counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;
		}

		template <typename data_type, size_manager_t size_manager>
		inline void realloc_more_primitive_types()
		{
			size_manager(counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// realloc data_pntr
			counts.data_pntr = (ubyte *)mem::safe_realloc_array<data_type>(
				(data_type *)counts.data_pntr, counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			counts.counts_pntr = mem::safe_realloc_array<size_t>(counts.counts_pntr, counts.size);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}

		template <typename data_type, size_manager_t size_manager>
		inline void increase_primitive_types(data_type value)
		{
			for (size_t index = 0; index < counts.len; index += 1)
			{
				if (((data_type *) counts.data_pntr)[index] == value)
				{
					counts.counts_pntr[index] += 1;
					return;
				}
			}

			if (counts.len == counts.size)
			{
				realloc_more_primitive_types<data_type, size_manager>();
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}

			((data_type *)counts.data_pntr)[counts.len] = value;
			counts.counts_pntr[counts.len] = 1;
			counts.len += 1;
		}

		// fixed bytes

		template <size_manager_t size_manager>
		inline void malloc_fixed_bytes()
		{
			size_t real_data_size;

			// counts size & bits
			size_manager(counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			counts.size = std::min<size_t>(info.possible_data_number, counts.size);
			counts.len = 0;

			// remaining size & bits
			remaining.bits = info.buffer_bits % info.bits;
			remaining.size = get_bytes_per_bits(info.buffer_bits);

			// real data size
			math::safe_mul(info.size, counts.size, real_data_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// --- malloc memory ---
			remaining.pntr = (ubyte *)mem::safe_malloc(remaining.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			counts.data_pntr = mem::safe_malloc(real_data_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			counts.counts_pntr = mem::safe_malloc_array(counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;
		}

		template <size_manager_t size_manager>
		inline void increase_fixed_bytes(ubyte *new_data_pntr)
		{
			ubyte *pntr = counts.data_pntr;
			ubyte *end = counts.data_pntr + (counts.len * info.size);

			size_t index = 0;

			while (pntr < end)
			{
				if (memcmp(pntr, new_data_pntr))
				{
					counts.counts_pntr[index] += 1;
					return;
				}

				index += 1;
				pntr += info.size;
			}

			
		}
	}; // struct segmented_buffer

	template <typename data_type, size_manager_t size_manager>
	static inline void count_primitive_types(ubuffer_t &buffer, segmented_buffer_t &result)
	{
		data_type *pntr;
		data_type *end;

		// malloc primitive types
		result.malloc_primitive_types<data_type, size_manager>();
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		// --- main process ---
		pntr = (data_type *)buffer.pntr;
		end = (data_type *)(buffer.pntr + buffer.size - (buffer.size % result.info.size));

		while (pntr < end)
		{
			result.increase_primitive_types<data_type, size_manager>(*pntr);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;
			pntr += 1;
		}

		// copy remaining
		memcpy(result.remaining.pntr, pntr, result.remaining.size);
	}

	template <size_manager_t size_manager>
	static inline void count_fixed_bytes(ubuffer_t &buffer, segmented_buffer_t &result)
	{
		ubyte *pntr;
		ubyte *end;

		// malloc primitive types
		result.malloc_fixed_bytes<size_manager>();
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		// --- main process ---
		pntr = buffer.pntr;
		end = buffer.pntr + buffer.size - (buffer.size % result.info.size);

		while (pntr < end)
		{
			result.increase_fixed_bytes(*pntr);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;
			pntr += result.info.size;
		}

		// copy remaining
		memcpy(result.remaining.pntr, pntr, result.remaining.size);
	}

	template <size_manager_t size_manager = double_size_manager>
	static inline void count_bits(size_t data_bits, ubuffer_t &buffer, segmented_buffer_t &result)
	{
		result.info.calculate(data_bits, buffer.size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

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
			else
			{

			}
		}
		else
		{
			err::set(err::INVALID_ARGUMENT);
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}
	}

	static inline void count_bits(size_t data_bits, const char *name, segmented_buffer_t &result)
	{
		scl::ubuffer_t buffer;

		scl::io::malloc_fopen_fread(name, (void **)&buffer.pntr, &buffer.size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		count_bits(data_bits, buffer, result);
		err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);

		free(buffer.pntr);
	}
}