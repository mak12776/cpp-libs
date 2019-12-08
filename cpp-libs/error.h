#pragma once

#include <cstdint>

#include "types.h"

using namespace scl;

namespace scl
{
	namespace error
	{
		typedef enum
		{
			NO_ERROR,
			ERROR_NO_MEMORY,

			ERROR_FOPEN,
			ERROR_FSEEK,
			ERROR_FTELL,

			ERROR_FREAD,
			ERROR_FWRITE
		} num_t;

		typedef enum
		{
			FLAG_NONE = 0x00,

			FLAG_FILE_NAME = 0x01,
			FLAG_FILE_MODE = 0x02,
			FLAG_STREAM = 0x04,
		} flags_t;

		inline void operator|=(flags_t flags, flags_t f2)
		{
			flags = static_cast<flags_t>(static_cast<uint32_t>(flags) | static_cast<uint32_t>(f2));
		}

		num_t num;

		struct
		{
			flags_t flags;

			const char *file_name;
			const char *file_mode;
			const FILE *stream;
		} info;

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
	}
}