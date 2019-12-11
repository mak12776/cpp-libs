#pragma once

#include <cstdio>

#include "error.h"
#include "types.h"
#include "buffer.h"

using namespace scl::error;
using namespace scl::buffer;

namespace scl
{
	namespace io
	{
		static inline FILE *safe_fopen(const char *name, const char *mode)
		{
			FILE *file = std::fopen(name, mode);

			if (file == NULL)
			{
				num = ERROR_FOPEN;
				set_info_file_mode(mode);
			}
			return file;
		}

		static inline long safe_ftell(FILE *stream)
		{
			long value;

			value = std::ftell(stream);
			if (value == -1)
			{
				num = ERROR_FTELL;
				set_info_stream(stream);
			}
			return value;
		}

		static inline void safe_fseek(FILE *stream, long int offset, int origin)
		{
			if (fseek(stream, offset, origin))
			{
				num = ERROR_FSEEK;
				set_info_stream(stream);
			}
		}

		static inline long get_file_size(FILE *stream)
		{
			long size;

			safe_fseek(stream, 0, SEEK_END);
			if (num) return 0;

			size = safe_ftell(stream);
			if (num) return 0;

			safe_fseek(stream, 0, SEEK_SET);
			if (num) return 0;

			return size;
		}

		static inline size_t safe_fread(void *pntr, size_t size, FILE *file)
		{
			size_t read_number;

			read_number = fread(pntr, 1, size, file);
			if (read_number != size)
			{
				num = ERROR_FREAD;
				set_info_stream(file);
			}
			return read_number;
		}

		static inline size_t safe_write(void *pntr, size_t size, FILE *file)
		{
			size_t write_number;

			write_number = fwrite(pntr, 1, size, file);
			if (write_number != size)
			{
				num = ERROR_FWRITE;
				set_info_stream(file);
			}
			return write_number;
		}

#if 0
		static inline void read_file(buffer_t *buffer, FILE *file)
		{
			return;
		}

		static inline void read_file_name(buffer_t *buffer, const char *name)
		{
			return;
		}
#endif

	}
}