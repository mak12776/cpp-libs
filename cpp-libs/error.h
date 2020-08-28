#pragma once

#include <cstdint>
#include <stdio.h>

namespace scl
{
	namespace err
	{
		enum class num_t : uint8_t
		{
			ERROR_NO_MEMORY, ERROR_NO_MEMORY_TYPE,
			ERROR_INVALID_ARGUMENT,
			ERROR_INT_OVERFLOW, ERROR_FLOAT_OVERFLOW,
			ERROR_FOPEN, ERROR_FTELL, ERROR_FSEEK,
			ERROR_FREAD, ERROR_FWRITE,
		};

		static inline const char *to_string(enum num_t num)
		{
			switch (num)
			{
			case num_t::ERROR_NO_MEMORY: return "NO_MEMORY";
			case num_t::ERROR_NO_MEMORY_TYPE: return "ERROR_NO_MEMORY_TYPE";
			case num_t::ERROR_INVALID_ARGUMENT: return "ERROR_INVALID_ARGUMENT";
			case num_t::ERROR_INT_OVERFLOW: return "ERROR_INT_OVERFLOW";
			case num_t::ERROR_FLOAT_OVERFLOW: return "ERROR_FLOAT_OVERFLOW";
			case num_t::ERROR_FOPEN: return "ERROR_FOPEN";
			case num_t::ERROR_FTELL: return "ERROR_FTELL";
			case num_t::ERROR_FSEEK: return "ERROR_FSEEK";
			case num_t::ERROR_FREAD: return "ERROR_FREAD";
			case num_t::ERROR_FWRITE: return "ERROR_FWRITE";
			default: return "UNKNOWN";
			}
		}

		constexpr size_t errors_length = 1024;
		size_t errors_index = 0;

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
		} errors[errors_length];

		static inline void clear()
		{
			errors_index = 0;
		}

		static inline bool check()
		{
			return errors_index != 0;
		}

		static inline const char *get_last_error()
		{
			if (errors_index == 0) return "NO ERROR";
			return to_string(errors[errors_index - 1].num);
		}

		// file name & line number

		static inline void set_file_info(const char *file_name, uint64_t line_number)
		{
			size_t index;

			if (errors_index != 0)
			{
				index = (errors_index == 0) ? errors_index : errors_index - 1;
				errors[index].file_name = file_name;
				errors[index].line_number = line_number;
			}
		}

		// error types

		static inline void set_no_memory(size_t size)
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_NO_MEMORY;
				errors[errors_index].info.memory.size = size;
				errors_index += 1;
			}
		}

		static inline void set_no_memory_type(size_t size, size_t type_size)
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_NO_MEMORY_TYPE;
				errors[errors_index].info.memory.size = size;
				errors[errors_index].info.memory.type_size = type_size;
				errors_index += 1;
			}
		}

		static inline void set_invalid_arguments(const char *function_name, const char *argument_name)
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_INVALID_ARGUMENT;
				errors[errors_index].info.invalid_argument.function_name = function_name;
				errors[errors_index].info.invalid_argument.argument_name = argument_name;
				errors_index += 1;
			}
		}

		static inline void set_fopen(const char *name, const char *mode)
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_FOPEN;
				errors[errors_index].info.fopen.name = name;
				errors[errors_index].info.fopen.mode = mode;
				errors_index += 1;
			}
		}

		static inline void set_ftell()
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_FTELL;
				errors_index += 1;
			}
		}

		static inline void set_fseek()
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_FSEEK;
				errors_index += 1;
			}
		}

		static inline void set_int_overflow(const char *operation_type)
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_INT_OVERFLOW;
				errors[errors_index].info.overflow.operation_type = operation_type;
				errors_index += 1;
			}
		}

		static inline void set_float_overflow(const char *operation_type)
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_FLOAT_OVERFLOW;
				errors[errors_index].info.overflow.operation_type = operation_type;
				errors_index += 1;
			}
		}

		static inline void set_fread(FILE *stream, size_t size)
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_FREAD;
				errors[errors_index].info.f_read_write.stream = stream;
				errors[errors_index].info.f_read_write.size = size;
				errors_index += 1;
			}
		}

		static inline void set_fwrite(FILE *stream, size_t size)
		{
			if (errors_index != errors_length)
			{
				errors[errors_index].num = num_t::ERROR_FWRITE;
				errors[errors_index].info.f_read_write.stream = stream;
				errors[errors_index].info.f_read_write.size = size;
				errors_index += 1;
			}
		}
	}
}