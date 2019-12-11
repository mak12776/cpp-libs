#pragma once

#include <cstdint>

namespace scl
{
	namespace error
	{
		// types

		typedef enum
		{
			NO_ERROR,
			ERROR_NO_MEMORY,

			ERROR_FOPEN,
			ERROR_FSEEK,
			ERROR_FTELL,

			ERROR_FREAD,
			ERROR_FWRITE,

			ERROR_BAD_ARGUMENT,
		} num_t;

		typedef uint32_t flags_t;

		const flags_t FLAG_NONE = 0x00;

		const flags_t FLAG_STREAM = 0x01;
		const flags_t FLAG_SIZE = 0x02;
		const flags_t FLAG_FILE_NAME = 0x04;
		const flags_t FLAG_FILE_MODE = 0x08;
		const flags_t FLAG_FUNCTION_NAME = 0x10;
		const flags_t FLAG_ARGUMENT_NAME = 0x20;

		num_t num;

		struct
		{
			flags_t flags;

			const FILE *stream;
			size_t size;

			const char *file_name;
			const char *file_mode;

			const char *function_name;
			const char *argument_name;
		} info;

		// functions

		static inline void clear_num()
		{
			num = NO_ERROR;
		}

		static inline void clear_info()
		{
			info.flags = FLAG_NONE;
		}

		static inline void set_info_file_name(const char *name)
		{
			info.flags |= FLAG_FILE_NAME;
			info.file_name = name;
		}

		static inline void set_info_file_mode(const char *mode)
		{
			info.flags |= FLAG_FILE_MODE;
			info.file_mode = mode;
		}

		static inline void set_info_stream(FILE *stream)
		{
			info.flags |= FLAG_STREAM;
			info.stream = stream;
		}

		static inline void set_info_size(const size_t size)
		{
			info.flags |= FLAG_SIZE;
			info.size = size;
		}

		static inline void set_info_function_name(const char *name)
		{
			info.flags |= FLAG_FUNCTION_NAME;
			info.function_name = name;
		}

		static inline void set_info_argument_name(const char *name)
		{
			info.flags |= FLAG_ARGUMENT_NAME;
			info.argument_name = name;
		}
	}
}