#pragma once

#include <cstdio>

#include "error.h"
#include "types.h"
#include "buffer.h"

namespace scl
{
	namespace io
	{
		static inline FILE *safe_fopen(const char *name, const char *mode)
		{
			FILE *file = std::fopen(name, mode);

			if (file == NULL)
				error::set_error_fopen(name, mode);

			return file;
		}

		static inline long safe_ftell(FILE *stream)
		{
			long value;

			value = std::ftell(stream);
			if (value == -1)
				error::set_error_ftell(stream);

			return value;
		}

		static inline void safe_fseek(FILE *stream, long int offset, int origin)
		{
			if (fseek(stream, offset, origin))
				error::set_error_fseek(stream);
		}

		static inline long get_file_size(FILE *stream)
		{
			long size;

			safe_fseek(stream, 0, SEEK_END);
			if (error::num) return 0;

			size = safe_ftell(stream);
			if (error::num) return 0;

			safe_fseek(stream, 0, SEEK_SET);
			if (error::num) return 0;

			return size;
		}

		static inline size_t safe_fread(void *pntr, size_t size, FILE *stream)
		{
			size_t read_number;

			read_number = fread(pntr, 1, size, stream);
			if (read_number != size)
				error::set_error_fread(stream);

			return read_number;
		}

		static inline size_t safe_write(void *pntr, size_t size, FILE *stream)
		{
			size_t write_number;

			write_number = fwrite(pntr, 1, size, stream);
			if (write_number != size)
				error::set_error_fwrite(stream);

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