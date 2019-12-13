#pragma once

#include <cstdint>

namespace scl
{
	namespace error
	{
		// types

		typedef enum
		{
			NO_ERROR = 0,

			ERROR_NO_MEMORY,

			ERROR_INT_OVERFLOW,
			ERROR_FLOAT_OVERFLOW,

			ERROR_FOPEN,
			ERROR_FSEEK,
			ERROR_FTELL,

			ERROR_FREAD,
			ERROR_FWRITE,

			ERROR_BAD_ARGUMENT,

			ERROR_BAD_ALLOC_EXCEPTION,
		} num_t;

		static inline const char *get_string(num_t num)
		{
			switch (num)
			{
#define ERROR_CASE(NAME) case NAME: return #NAME;

				ERROR_CASE(NO_ERROR);

				ERROR_CASE(ERROR_NO_MEMORY);

				ERROR_CASE(ERROR_INT_OVERFLOW);
				ERROR_CASE(ERROR_FLOAT_OVERFLOW);

				ERROR_CASE(ERROR_FOPEN);
				ERROR_CASE(ERROR_FSEEK);
				ERROR_CASE(ERROR_FTELL);

				ERROR_CASE(ERROR_FREAD);
				ERROR_CASE(ERROR_FWRITE);

				ERROR_CASE(ERROR_BAD_ARGUMENT);

#undef ERROR_CASE
			default: return "UNKNOWN ERROR";
			}
		}

#if 0
		typedef uint32_t flags_t;

		const flags_t FLAG_NONE = 0x00;

		const flags_t FLAG_STREAM = 0x01;
		const flags_t FLAG_SIZE = 0x02;
		const flags_t FLAG_FILE_NAME = 0x04;
		const flags_t FLAG_FILE_MODE = 0x08;
		const flags_t FLAG_FUNCTION_NAME = 0x10;
		const flags_t FLAG_ARGUMENT_NAME = 0x20;
#endif

		// variables

		num_t num = NO_ERROR;

		struct
		{
			/* usages are shown by following functions*/
			const char *file_name;
			const char *file_mode;
			size_t size;
			const FILE *stream;
			const char *function_name;
			const char *argument_name;
			const char *type_name;
		} info;

		const size_t messages_size = 1024;
		size_t messages_index = 0;

		char *messages[messages_size];
		
		// functions

		static inline void check_error()
		{
			if (num == NO_ERROR)
				return;

			printf("error: %s", get_string(num));
			exit(EXIT_ERROR);
		}

		static inline void clear_error()
		{
			num = NO_ERROR;
		}

		static inline void set_error_no_memory(size_t size)
		{
			num = ERROR_NO_MEMORY;
			info.size = size;
		}

		static inline void set_error_fopen(const char *name, const char *mode)
		{
			num = ERROR_FOPEN;
			info.file_name = name;
			info.file_mode = mode;
		}

		static inline void set_error_ftell(FILE *stream)
		{
			num = ERROR_FTELL;
			info.stream = stream;
		}

		static inline void set_error_fseek(FILE *stream)
		{
			num = ERROR_FSEEK;
			info.stream = stream;
		}

		static inline void set_error_fread(FILE *stream)
		{
			num = ERROR_FREAD;
			info.stream = stream;
		}

		static inline void set_error_fwrite(FILE *stream)
		{
			num = ERROR_FWRITE;
			info.stream = stream;
		}

		static inline void set_error_int_overflow()
		{
			num = ERROR_INT_OVERFLOW;
		}

		static inline void set_error_float_overflow()
		{
			num = ERROR_FLOAT_OVERFLOW;
		}

		static inline void set_error_bad_argument(const char *function_name, const char *argument_name)
		{
			num = ERROR_BAD_ARGUMENT;
			info.function_name = function_name;
			info.argument_name = argument_name;
		}

		static inline void set_error_bad_alloc_exception(const char *type_name, size_t size)
		{
			num = ERROR_BAD_ALLOC_EXCEPTION;
			info.type_name = type_name;
			info.size = size;
		}
	}
}