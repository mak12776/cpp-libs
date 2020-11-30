#pragma once

#include "scl/all.h"

namespace comp
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

	// segmented

	template <size_manager_t size_manager = comp::double_size_manager>
	struct thread_segmented_buffer_t
	{
		size_t data_bits;
		size_t data_size;

		size_t size;
		size_t len;
		size_t data_block_size;

		void *data_pntr;
		size_t *count_pntr;

		static inline void count_buffer(ubuffer_t &buffer)
		{

		}
	};

	template <size_manager_t size_manager = comp::double_size_manager>
	struct segmented_buffer_t
	{
		size_t buffer_bits;
		size_t buffer_size;

		size_t data_bits;
		size_t data_size;
		
		size_t possible_data_number;
		size_t total_data_number;

		size_t size;
		size_t length;
		size_t data_block_size;

		size_t remaining_bits;
		size_t remaining_size;

		void *data_pntr = nullptr;
		size_t *count_pntr = nullptr;

		void *remaining_pntr = nullptr;

		inline void allocate()
		{
			this->size = 0;

			// size, length
			size_manager(this->size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			this->size = (std::min)(this->size, this->possible_data_number);
			this->length = 0;

			// data block size
			math::safe_mul(this->size, this->data_size, this->data_block_size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// set remaining bits, size
			this->remaining_bits = this->buffer_bits % this->data_bits;
			this->remaining_size = get_bytes_per_bits(this->remaining_bits);

			// allocate remaining
			this->remaining_pntr = mem::safe_malloc(this->remaining_size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// allocate data_pntr
			this->data_pntr = mem::safe_malloc(this->data_block_size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// allocate count_pntr
			this->count_pntr = mem::safe_malloc_array<size_t>(this->size);
			if (err::check_push_file_info(ERR_ARGS))
				return;
		}

		inline void reallocate_more()
		{
			// get new size
			size_manager(this->size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// data block size
			math::safe_mul(this->size, this->data_size, this->data_block_size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// reallocate data_pntr
			mem::safe_realloc(&(this->data_pntr), this->data_block_size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// reallocate count_pntr
			mem::safe_realloc_array<size_t>(&(this->count_pntr), this->size);
			if (err::check_push_file_info(ERR_ARGS))
				return;
		}

		// fixed bytes

		inline void increase_fixed_bytes(ubyte *value_pntr)
		{
			ubyte *pntr = (ubyte *)this->data_pntr;
			ubyte *end = (ubyte *)this->data_pntr + (this->length * this->data_size);
			size_t index = 0;

			while (pntr < end)
			{
				if (memcmp(pntr, value_pntr, this->data_size) == 0)
				{
					this->count_pntr[index] += 1;
					return;
				}

				pntr += this->data_size;
				index += 1;
			}

			if (this->length == this->size)
			{
				reallocate_more();
				if (err::check_push_file_info(ERR_ARGS))
					return;
			}

			memcpy(end, value_pntr, this->data_size);
			this->count_pntr[this->length] = 1;
			this->length += 1;
		}

		inline void count_fixed_bytes(ubuffer_t &buffer)
		{
			ubyte *pntr = buffer.pntr;
			ubyte *end = buffer.pntr + buffer.size - this->remaining_size;

			allocate();
			if (err::check_push_file_info(ERR_ARGS))
				return;

			while (pntr < end)
			{
				increase_fixed_bytes(pntr);
				if (err::check_push_file_info(ERR_ARGS))
					return;

				pntr += this->data_size;
			}

			memcpy(this->remaining_pntr, end, this->remaining_size);
		}

		// primitive types

		template <typename data_type>
		inline void increase_primitive_types(data_type value)
		{
			for (size_t index = 0; index < this->length; index += 1)
			{
				if (((data_type *)this->data_pntr)[index] == value)
				{
					this->count_pntr[index] += 1;
					return;
				}
			}

			if (this->length == this->size)
			{
				reallocate_more();
				if (err::check_push_file_info(ERR_ARGS))
					return;
			}

			((data_type *)this->data_pntr)[this->length] = value;
			this->count_pntr[this->length] = 1;
			this->length += 1;
		}

		template <typename data_type>
		inline void count_primitive_types(ubuffer_t &buffer)
		{
			data_type *pntr = (data_type *)buffer.pntr;
			data_type *end = (data_type *)(buffer.pntr + buffer.size - this->remaining_size);

			allocate();
			if (err::check_push_file_info(ERR_ARGS))
				return;

			while (pntr < end)
			{
				increase_primitive_types(*pntr);
				if (err::check_push_file_info(ERR_ARGS))
					return;

				pntr += 1;
			}

			memcpy(this->remaining_pntr, end, this->remaining_size);
		}

		// main functions

		inline void count_buffer(size_t data_bits, ubuffer_t &buffer)
		{
			// buffer size, bits
			this->buffer_size = buffer.size;

			math::safe_mul(buffer.size, (size_t)8, this->buffer_bits);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// data bits, size
			this->data_bits = data_bits;
			this->data_size = get_bytes_per_bits(data_bits);

			// possible data number
			math::pow<size_t>(2, data_bits, this->possible_data_number);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// total data number
			this->total_data_number = this->buffer_bits / this->data_bits;

			if (data_bits % 8 == 0)
			{
				switch (this->data_size)
				{
				case sizeof uint8_t:
					this->count_primitive_types<uint8_t>(buffer);
					break;
				case sizeof uint16_t:
					this->count_primitive_types<uint16_t>(buffer);
					break;
				case sizeof uint32_t:
					this->count_primitive_types<uint32_t>(buffer);
					break;
				case sizeof uint64_t:
					this->count_primitive_types<uint64_t>(buffer);
					break;
				default:
					this->count_fixed_bytes(buffer);
					break;
				}
			}
			else
			{
				err::set(err::INVALID_ARGUMENT);
				err::push_file_info(ERR_ARGS);
			}
		}

		inline void count_file(size_t data_bits, FILE *file)
		{
			ubuffer_t buffer;

			buffer.allocate_fread(file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			count_buffer(data_bits, buffer);
			err::check_push_file_info(file);
		}

		inline void count_name(size_t data_bits, const char *name)
		{
			ubuffer_t buffer;
			
			buffer.allocate_fopen_fread(name);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			count_buffer(data_bits, buffer);
			err::check_push_file_info(ERR_ARGS);
		}
	};

	// --------------------------------------------------------------

	// segmented buffer

	template <size_manager_t size_manager = comp::double_size_manager>
	struct old_segmented_buffer_t
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
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
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
			info.calculate(data_bits, buffer.size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			// --- [main process] ---
			if (info.data_bits % 8 == 0)
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
}