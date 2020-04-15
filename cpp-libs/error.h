#pragma once

#include <cstdint>
#include <stdio.h>

namespace scl
{
	namespace error
	{
		enum class num_t : uint8_t
		{
			ERROR_NO_MEMORY, ERROR_NO_MEMORY_TYPE,

			ERROR_INVALID_ARGUMENT,

			ERROR_INT_OVERFLOW, ERROR_FLOAT_OVERFLOW,

			ERROR_FOPEN, ERROR_FTELL, ERROR_FSEEK,

			ERROR_FREAD, ERROR_FWRITE,
		};

		constexpr size_t errors_array_size = 1024;
		size_t errors_array_index = 0;

		struct
		{
			num_t num;
			const char *file_name;
			uint64_t line_number;

			union
			{
				struct
				{
					const char *function_name;
					const char *argument_name;
				} invalid_argument;

				struct
				{
					size_t size;
					size_t type_size;
				} memory;

				struct
				{
					const char *name;
					const char *mode;
				} fopen;

				struct
				{
					const char *operation_type;
				} overflow;

				struct
				{
					FILE *stream;
					size_t size;
				} f_read_write;
			} info;
		} errors[errors_array_size];

		static inline void clear()
		{
			errors_array_index = 0;
		}

		static inline bool check()
		{
			return errors_array_index != 0;
		}

		// file name & line number

		static inline void set_file_info(const char *file_name, uint64_t line_number)
		{
			if (errors_array_index != 0)
			{
				errors[errors_array_index].file_name = file_name;
				errors[errors_array_index].line_number = line_number;
			}
		}

		// error types

		static inline void set_no_memory(size_t size)
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_NO_MEMORY;
				errors[errors_array_index].info.memory.size = size;
				errors_array_index += 1;
			}
		}

		static inline void set_no_memory_type(size_t size, size_t type_size)
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_NO_MEMORY_TYPE;
				errors[errors_array_index].info.memory.size = size;
				errors[errors_array_index].info.memory.type_size = type_size;
				errors_array_index += 1;
			}
		}

		static inline void set_invalid_arguments(const char *function_name, const char *argument_name)
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_INVALID_ARGUMENT;
				errors[errors_array_index].info.invalid_argument.function_name = function_name;
				errors[errors_array_index].info.invalid_argument.argument_name = argument_name;
				errors_array_index += 1;
			}
		}

		static inline void set_fopen(const char *name, const char *mode)
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_FOPEN;
				errors[errors_array_index].info.fopen.name = name;
				errors[errors_array_index].info.fopen.mode = mode;
				errors_array_index += 1;
			}
		}

		static inline void set_ftell()
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_FTELL;
				errors_array_index += 1;
			}
		}

		static inline void set_fseek()
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_FSEEK;
				errors_array_index += 1;
			}
		}

		static inline void set_int_overflow(const char *operation_type)
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_INT_OVERFLOW;
				errors[errors_array_index].info.overflow.operation_type = operation_type;
				errors_array_index += 1;
			}
		}

		static inline void set_float_overflow(const char *operation_type)
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_FLOAT_OVERFLOW;
				errors[errors_array_index].info.overflow.operation_type = operation_type;
				errors_array_index += 1;
			}
		}

		static inline void set_fread(FILE *stream, size_t size)
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_FREAD;
				errors[errors_array_index].info.f_read_write.stream = stream;
				errors[errors_array_index].info.f_read_write.size = size;
				errors_array_index += 1;
			}
		}

		static inline void set_fwrite(FILE *stream, size_t size)
		{
			if (errors_array_index != errors_array_size)
			{
				errors[errors_array_index].num = num_t::ERROR_FWRITE;
				errors[errors_array_index].info.f_read_write.stream = stream;
				errors[errors_array_index].info.f_read_write.size = size;
				errors_array_index += 1;
			}
		}
	}
}