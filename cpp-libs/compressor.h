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

	enum flag_t : uint32_t
	{
		POINTER_BASED = 0,
		DATA_BASED = 1,
		POINTER_DATA_MASK = 1,
	};

	template <size_manager_t size_manager>
	struct data_count_t
	{
		size_t data_bits;
		size_t data_size;

		size_t size;
		size_t length;

		flag_t flag;

		union
		{
			struct
			{
				ubuffer_t *buffer;
				void **pntr_array;
			};
			struct
			{
				size_t data_block_size;
				void *data_pntr;
			};
		};

		size_t *count_pntr;

		// safe read write

		inline void safe_fwrite(FILE *file)
		{
			// data bits
			io::fwrite_data(this->data_bits, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// data size
			io::fwrite_data(this->data_size, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// flag
			io::fwrite_data(this->flag, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// length
			io::fwrite_data(this->length, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// "data pntr" or "pntr array"
			if ((flag & flag_t::POINTER_DATA_MASK) == flag_t::DATA_BASED) 
			{ // DATA_BASED

			}
				io::safe_fwrite(this->data_pntr, this->data_size * this->length, file);
			else
			{ // POINTER_BASED
				uintptr_t diff;
				for (size_t index = 0; index < this->length; index += 1)
				{
				}
			}
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::fwrite_array<size_t>(this->count_pntr, this->length, file);
			err::check_push_file_info(ERR_ARGS);
		}

		inline void safe_fread(FILE *file)
		{
			// data bits
			io::fwrite_data(this->data_bits, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// data size
			io::fwrite_data(this->data_size, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// flag
			io::fwrite_data(this->flag, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// length
			io::fwrite_data(this->length, file);
			if (err::check_push_file_info(ERR_ARGS))
				return
			this->size = this->length;

			// "data pntr" or "pntr array"
			if ((flag & flag_t::POINTER_DATA_MASK) == flag_t::DATA_BASED)
			{
				math::safe_mul(this->length, this->data_size, this->data_block_size);
				if (err::check_push_file_info(ERR_ARGS))
					return;

				io::malloc_fread(file, &(this->data_pntr), this->data_block_size);
			}
			else
				io::malloc_fread_array<void *>(file, &(this->pntr_array), this->length);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			// count pntr
			io::malloc_fread_array<size_t>(file, &(this->count_pntr), this->length);
			err::check_push_file_info(ERR_ARGS);
		}

		// memory managment

		template <bool is_data>
		inline void allocate(size_t suggested_size = SIZE_MAX)
		{
			this->size = 0;

			// size, length
			size_manager(this->size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			this->size = (std::min)(this->size, suggested_size);
			this->length = 0;

			if constexpr (is_data)
			{
				// data block size
				math::safe_mul(this->size, this->data_size, this->data_block_size);
				if (err::check_push_file_info(ERR_ARGS))
					return;

				// allocate data_pntr
				this->data_pntr = mem::safe_malloc(this->data_block_size);
				if (err::check_push_file_info(ERR_ARGS))
					return;
			}
			else
			{
				this->pntr_array = mem::safe_malloc_array<void *>(this->size);
				if (err::check_push_file_info(ERR_ARGS))
					return;
			}

			// allocate count_pntr
			this->count_pntr = mem::safe_malloc_array<size_t>(this->size);
			err::check_push_file_info(ERR_ARGS);
		}

		template <bool is_data>
		inline void reallocate_more()
		{
			// get new size
			size_manager(this->size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			if constexpr (is_data)
			{
				// data block size
				math::safe_mul(this->size, this->data_size, this->data_block_size);
				if (err::check_push_file_info(ERR_ARGS))
					return;

				// reallocate data_pntr
				mem::safe_realloc(&(this->data_pntr), this->data_block_size);
				if (err::check_push_file_info(ERR_ARGS))
					return;
			}
			else
			{
				mem::safe_realloc_array<void *>(&(this->pntr_array), this->size);
				if (err::check_push_file_info(ERR_ARGS))
					return;
			}

			// reallocate count_pntr
			mem::safe_realloc_array<size_t>(&(this->count_pntr), this->size);
			err::check_push_file_info(ERR_ARGS);
		}

		// fixed bytes or pointer 

		inline void increase_pointer(void *value_pntr)
		{
			size_t index = 0;
			while (index < this->length)
			{
				if (memcmp(this->pntr_array[index], value_pntr, this->data_size) == 0)
				{
					this->count_pntr[index] += 1;
					return;
				}
				index += 1;
			}

			if (this->length == this->size)
			{
				reallocate_more<false>();
				if (err::check_push_file_info(ERR_ARGS))
					return;
			}

			this->pntr_array[this->length] = value_pntr;
			this->count_pntr[this->length] = 1;
			this->length += 1;
		}

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
				reallocate_more<true>();
				if (err::check_push_file_info(ERR_ARGS))
					return;
			}

			memcpy(end, value_pntr, this->data_size);
			this->count_pntr[this->length] = 1;
			this->length += 1;
		}

		template <bool is_data>
		inline void count_fixed_bytes_or_pointer(ubuffer_t &buffer)
		{
			ubyte *pntr = buffer.pntr;
			ubyte *end = buffer.pntr + buffer.size;

			allocate<is_data>();
			if (err::check_push_file_info(ERR_ARGS))
				return;

			while (pntr < end)
			{
				if constexpr (is_data)
					increase_fixed_bytes(pntr);
				else
					increase_pointer((void *)pntr);
				if (err::check_push_file_info(ERR_ARGS))
					return;

				pntr += this->data_size;
			}
		}

		// primitive types

		template <typename data_type>
		inline void increase_primitive_types(data_type value)
		{
			data_type *pntr = (data_type *)this->data_pntr;
			for (size_t index = 0; index < this->length; index += 1)
			{
				if (pntr[index] == value)
				{
					this->count_pntr[index] += 1;
					return;
				}
			}

			if (this->length == this->size)
			{
				reallocate_more<true>();
				if (err::check_push_file_info(ERR_ARGS))
					return;
				pntr = (data_type *)this->data_pntr;
			}

			pntr[this->length] = value;
			this->count_pntr[this->length] = 1;
			this->length += 1;
		}

		template <typename data_type>
		inline void count_primitive_types(ubuffer_t &buffer)
		{
			data_type *pntr = (data_type *)buffer.pntr;
			data_type *end = (data_type *)(buffer.pntr + buffer.size);

			allocate<true>();
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
				if (this->data_size > sizeof(size_t))
				{
					this->flag = static_cast<flag_t>((this->flag & ~flag_t::POINTER_DATA_MASK) | (flag_t::POINTER_BASED));
					count_fixed_bytes_or_pointer<false>(buffer);
				}
				else
				{
					this->flag = static_cast<flag_t>((this->flag & ~flag_t::POINTER_DATA_MASK) | flag_t::DATA_BASED);
					switch (this->data_size)
					{
					case sizeof uint8_t:
						count_primitive_types<uint8_t>(buffer);
						break;
					case sizeof uint16_t:
						count_primitive_types<uint16_t>(buffer);
						break;
					case sizeof uint32_t:
						count_primitive_types<uint32_t>(buffer);
						break;
					case sizeof uint64_t:
						count_primitive_types<uint64_t>(buffer);
						break;
					default:
						count_fixed_bytes_or_pointer<true>(buffer);
						break;
					}
				}
				err::check_push_file_info(ERR_ARGS);
			}
			else
			{
				err::set(err::INVALID_ARGUMENT);
				err::push_file_info(ERR_ARGS);
			}
		}

		static inline void static_count_buffer(size_t data_bits, ubuffer_t &buffer, data_count_t result)
		{
			result.count_buffer(data_bits, &buffer);
			err::check_push_file_info(ERR_ARGS);
		}
	};

	struct remaining_t
	{
		size_t bits;
		size_t size;

		void *pntr;

		inline void allocate_copy(size_t bits, ubyte *buffer_pntr)
		{
			this->bits = bits;
			this->size = get_bytes_per_bits(this->bits);

			if (this->size == 0)
				return;

			this->pntr = mem::safe_malloc(this->size);
			err::check_push_file_info(ERR_ARGS);

			if (this->bits % 8 == 0)
				memcpy(this->pntr, buffer_pntr, this->size);
			else
			{
				err::set(err::INVALID_ARGUMENT);
				err::push_file_info(ERR_ARGS);
			}
		}

		inline void safe_fwrite(FILE *file)
		{
			io::fwrite_data(this->bits, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::fwrite_data(this->size, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::safe_fwrite(this->pntr, this->size, file);
			err::check_push_file_info(ERR_ARGS);
		}

		inline void safe_fread(FILE *file)
		{
			io::fread_data(this->bits, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::fread_data(this->size, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::malloc_fread(file, &(this->pntr), this->size);
			err::check_push_file_info(ERR_ARGS);
		}
	};

	struct info_t
	{
		size_t buffer_bits;
		size_t buffer_size;

		size_t possible_data_number;
		size_t total_data_count;

		inline void initalize(size_t data_bits, size_t buffer_size)
		{
			this->buffer_size = buffer_size;

			math::safe_mul(buffer_size, (size_t)8, this->buffer_bits);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			math::safe_pow<size_t>(2, data_bits, this->possible_data_number);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			this->total_data_count = this->buffer_bits / data_bits;
		}

		inline void safe_fwrite(FILE *file)
		{
			io::fwrite_data(this->buffer_bits, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::fwrite_data(this->buffer_size, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::fwrite_data(this->possible_data_number, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::fwrite_data(this->total_data_count, file);
			err::check_push_file_info(ERR_ARGS);
		}

		inline void safe_fread(FILE *file)
		{
			io::fread_data(this->buffer_bits, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::fread_data(this->buffer_size, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::fread_data(this->possible_data_number, file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			io::fread_data(this->total_data_count, file);
			err::check_push_file_info(ERR_ARGS);
		}
	};

	template <size_manager_t size_manager = comp::double_size_manager>
	struct segmented_buffer_t
	{
		info_t info;
		data_count_t<size_manager> data_count;
		remaining_t remaining;

		// safe fread fwrite

		inline void safe_fread(FILE *file)
		{
			info.safe_fread(file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			data_count.safe_fread(file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			remaining.safe_fread(file);
			err::check_push_file_info(ERR_ARGS);
		}

		inline void safe_fwrite(FILE *file)
		{
			info.safe_fread(file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			data_count.safe_fread(file);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			remaining.safe_fread(file);
			err::check_push_file_info(ERR_ARGS);
		}

		// main functions

		template <size_t thread_number = 1>
		inline void count_buffer(size_t data_bits, ubuffer_t &buffer)
		{
			this->info.initalize(data_bits, buffer.size);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			this->remaining.allocate_copy(this->info.buffer_bits % data_bits, buffer.pntr);
			if (err::check_push_file_info(ERR_ARGS))
				return;

			ubuffer_t count_buffer(buffer.pntr, buffer.size - this->remaining.size);

			if constexpr (thread_number == 1)
			{
				data_count.count_buffer(data_bits, count_buffer);
				err::check_push_file_info(ERR_ARGS);
				return;
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