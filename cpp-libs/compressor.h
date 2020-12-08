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

	template <size_manager_t size_manager>
	struct pointer_count_t
	{
		size_t data_bits;
		size_t data_size;

		size_t size;
		size_t len;
		
		void **pntr_pntr;
		size_t *count_pntr;
	};

	template <size_manager_t size_manager>
	struct data_count_t
	{
		size_t data_bits;
		size_t data_size;

		size_t size;
		size_t length;
		size_t data_block_size;

		void *data_pntr;
		size_t *count_pntr;

		inline void allocate(size_t suggested_size = SIZE_MAX)
		{
			this->size = 0;

			// size, length
			size_manager(this->size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			this->size = (std::min)(this->size, suggested_size);
			this->length = 0;

			// data block size
			math::safe_mul(this->size, this->data_size, this->data_block_size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// allocate data_pntr
			this->data_pntr = mem::safe_malloc(this->data_block_size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// allocate count_pntr
			this->count_pntr = mem::safe_malloc_array<size_t>(this->size);
			err::check_push_file_info(ERR_ARGS);
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
			err::check_push_file_info(ERR_ARGS);
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
		}

		// count buffer

		inline void count_buffer(size_t data_bits, ubuffer_t &buffer)
		{
			this->data_bits = data_bits;
			this->data_size = get_bytes_per_bits(data_bits);

			if (data_bits % 8 == 0)
			{

			}
			else
			{
				err::set(err::INVALID_ARGUMENT);
				err::push_file_info(ERR_ARGS);
			}
		}
	};

	union data_pointer_count_t
	{
		
	};

	struct remaining_t
	{
		size_t bits;
		size_t size;

		void *pntr;

		inline void allocate(size_t buffer_bits, size_t data_bits, ubyte *buffer_pntr)
		{
			this->bits = buffer_bits % data_bits;
			this->size = get_bytes_per_bits(this->bits);

			if (this->size == 0)
				return;

			this->pntr = mem::safe_malloc(this->size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			/*if (this->remaining_bits % 8 == 0)
				memcpy(this->remaining_pntr, end, this->remaining_size);
			else
			{
				err::set(err::INVALID_ARGUMENT);
				err::push_file_info(ERR_ARGS);
			}*/
		}
	};

	enum class flag_t
	{
		POINTER_BASED = 0,
		DATA_BASED = 1,
	};

	template <size_manager_t size_manager = comp::double_size_manager>
	struct segmented_buffer_t
	{
		// buffer informations
		size_t buffer_bits;
		size_t buffer_size;

		size_t possible_data_number;
		size_t total_data_count;

		flag_t flag;
		union
		{
			data_count_t data_count;
			pointer_count_t pointer_count;
		};
		
		remaining_t remaining;

		// main functions

		inline void initial_values(size_t data_bits, size_t buffer_size)
		{
			// buffer size, bits
			this->buffer_size = buffer_size;
			math::safe_mul(buffer_size, (size_t)8, this->buffer_bits);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// possible data number
			math::pow<size_t>(2, data_bits, this->possible_data_number);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// total data number
			this->total_data_count = this->buffer_bits / this->data_bits;
		}

		template <size_t threads_number = 1>
		inline void count_buffer(size_t data_bits, ubuffer_t &buffer)
		{
			initial_values(data_bits, buffer_size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			if constexpr (threads_number == 1)
			{
				if (data_bits % 8 == 0)
				{
					
				}
			}
			else
			{
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
}