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

	template <size_manager_t size_manager = bith::double_size_manager>
	struct fixed_data_counts
	{
		size_t data_bits;
		size_t data_size;
		size_t data_block_size;

		size_t size;
		size_t len;

		void *data_pntr = nullptr;
		size_t *counts_pntr = nullptr;

		inline void malloc(size_t data_size, size_t suggested_initial_block_size)
		{
			// get size
			size_manager(this->size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			this->size = std::min<size_t>(this->size, suggested_initial_block_size);
			this->len = 0;

			// calculate data size & data block size
			this->data_size = data_size;
			math::safe_mul(this->size, data_size, this->data_block_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// malloc data_pntr
			this->data_pntr = mem::safe_malloc(this->data_block_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// malloc counts_pntr
			this->counts_pntr = mem::safe_malloc_array<size_t>(size);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}

		inline void realloc_more()
		{
			// get new size
			size_manager(this->size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				mem::free(this->data_pntr);

			// calculate data block size
			math::safe_mul(this->size, this->data_size, this->data_block_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// realloc data_pntr
			mem::safe_realloc(&(this->data_pntr), this->data_block_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// realloc counts_pntr
			mem::safe_realloc_array<size_t>(&(this->counts_pntr), this->size);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}

		template <typename data_type>
		inline void increase_primitive_types(data_type value)
		{
			for (size_t index = 0; index < this->len; index += 1)
			{
				if (((data_type *)this->data_pntr)[index] == value)
				{
					this->counts_pntr[index] += 1;
					return;
				}
			}

			if (this->len == this->size)
			{
				realloc_more();
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}

			((data_type *)this->data_pntr)[this->len] = value;
			this->counts_pntr[this->len] = 1;
			this->len += 1;
		}

		inline void increase_fixed_bytes(void *value_pntr)
		{
			ubyte *pntr = (ubyte *)this->data_pntr;
			ubyte *end = (ubyte *)this->data_pntr + (this->len * this->data_size);
			size_t index = 0;

			while (pntr < end)
			{
				if (memcmp(pntr, value_pntr, this->data_size))
				{
					this->counts_pntr[index] += 1;
					return;
				}

				pntr += this->data_size;
				index += 1;
			}

			if (this->len == this->size)
			{
				realloc_more();
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}

			memcpy(end, value_pntr, this->data_size);
			this->counts_pntr[this->len] = 1;
			this->len += 1;
		}
	};

	// segmented buffer

	template <size_manager_t size_manager = bith::double_size_manager>
	struct segmented_buffer_t
	{
		struct
		{
			size_t buffer_bits;
			size_t buffer_size;

			size_t data_bits;
			size_t data_size;

			size_t possible_data_number;

			inline void calculate(size_t data_bits, size_t buffer_size)
			{
				// calculating buffer informations
				this->buffer_size = buffer_size;

				math::safe_mul(buffer_size, (size_t)8, this->buffer_bits);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;

				// calculating data informations
				this->data_bits = data_bits;
				this->data_size = get_bytes_per_bits(data_bits);

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
			size_t data_block_size;

			void *data_pntr = nullptr;
			size_t *counts_pntr = nullptr;
		} counts;

		struct
		{
			size_t bits;
			size_t size;

			ubyte *pntr = nullptr;
		} remaining;

		// --- malloc remaining ---

		inline void malloc_remaining()
		{
			// remaining size & bits
			remaining.bits = info.buffer_bits % info.data_bits;
			remaining.size = get_bytes_per_bits(remaining.bits);

			// --- allocate memory ---
			remaining.pntr = (ubyte *)mem::safe_malloc(remaining.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;
		}

		// --- malloc counts ---

		inline void malloc_counts()
		{
			size_manager(counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			counts.size = (std::min)(info.possible_data_number, counts.size);
			counts.len = 0;

			// calculate data block size
			math::safe_mul(counts.size, info.data_size, counts.data_block_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// malloc data_pntr
			counts.data_pntr = mem::safe_malloc(counts.data_block_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// malloc counts_pntr
			counts.counts_pntr = mem::safe_malloc_array<size_t>(counts.size);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}

		inline void realloc_more()
		{
			// get new size
			size_manager(counts.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// calculate data block size
			math::safe_mul(counts.size, info.data_size, counts.data_block_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// realloc data_pntr
			mem::safe_realloc(&counts.data_pntr, counts.data_block_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			mem::safe_realloc_array<size_t>(&counts.counts_pntr, counts.size);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}

		// increase primitve types, fixed bytes

		template <typename data_type>
		inline void increase_primitive_types(data_type value)
		{
			for (size_t index = 0; index < counts.len; index += 1)
			{
				if (((data_type *)counts.data_pntr)[index] == value)
				{
					counts.counts_pntr[index] += 1;
					return;
				}
			}

			if (counts.len == counts.size)
			{
				realloc_more();
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}

			((data_type *)counts.data_pntr)[counts.len] = value;
			counts.counts_pntr[counts.len] = 1;
			counts.len += 1;
		}

		inline void increase_fixed_bytes(void *value_pntr)
		{
			ubyte *pntr = (ubyte *)counts.data_pntr;
			ubyte *end = (ubyte *)counts.data_pntr + (counts.len * info.data_size);
			size_t index = 0;

			while (pntr < end)
			{
				if (memcmp(pntr, value_pntr, info.data_size) == 0)
				{
					counts.counts_pntr[index] += 1;
					return;
				}

				pntr += info.data_size;
				index += 1;
			}

			if (counts.len == counts.size)
			{
				realloc_more();
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}

			memcpy(end, value_pntr, info.data_size);
			counts.counts_pntr[counts.len] = 1;
			counts.len += 1;
		}

		// --- count primitive types, fixed bytes ---

		template <typename data_type>
		inline void count_primitive_types(ubuffer_t &buffer)
		{
			data_type *pntr;
			data_type *end;

			// malloc counts
			malloc_counts();
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// malloc remaining
			malloc_remaining();
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// --- main process ---
			pntr = (data_type *)buffer.pntr;
			end = (data_type *)(buffer.pntr + buffer.size - (buffer.size % info.size));

			while (pntr < end)
			{
				increase_primitive_types<data_type>(*pntr);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
				pntr += 1;
			}

			// copy remaining
			memcpy(remaining.pntr, pntr, remaining.size);
		}

		inline void count_fixed_bytes(ubuffer_t &buffer)
		{
			ubyte *pntr;
			ubyte *end;

			// malloc counts
			malloc_counts();
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// malloc remaining
			malloc_remaining();
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// --- main process ---
			pntr = buffer.pntr;
			end = buffer.pntr + buffer.size - (buffer.size % info.data_size);

			while (pntr < end)
			{
				increase_fixed_bytes(pntr);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
				pntr += info.data_size;
			}

			// copy remaining
			memcpy(remaining.pntr, pntr, remaining.size);
		}

		// count buffer

		inline void count_buffer(size_t data_bits, ubuffer_t &buffer)
		{
			this->info.calculate(data_bits, buffer.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// --- [main process] ---
			if (this->info.data_bits % 8 == 0)
			{
				if (info.data_size == sizeof(uint8_t))
				{
					count_primitive_types<uint8_t>(buffer);
					err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else if (info.data_size == sizeof(uint16_t))
				{
					count_primitive_types<uint16_t>(buffer);
					err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else if (info.data_size == sizeof(uint32_t))
				{
					count_primitive_types<uint32_t>(buffer);
					err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else if (info.data_size == sizeof(uint64_t))
				{
					count_primitive_types<uint64_t>(buffer);
					err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else
				{
					count_fixed_bytes(buffer);
					err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
			}
			else
			{
				err::set(err::INVALID_ARGUMENT);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}
	}; // struct segmented_buffer

	template <size_manager_t size_manager = double_size_manager>
	static inline void count_bits(size_t data_bits, ubuffer_t &buffer, segmented_buffer_t<size_manager> &result)
	{
		result.info.calculate(data_bits, buffer.size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		
	}

	template <size_manager_t size_manager>
	static inline void count_bits(size_t data_bits, const char *name, segmented_buffer_t<size_manager> &result)
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