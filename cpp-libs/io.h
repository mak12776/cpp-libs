#pragma once

#include <cstdio>

#include "old_error.h"
#include "types.h"

namespace scl
{
	namespace io
	{
		static inline FILE *safe_fopen(const char *name, const char *mode)
		{
			FILE *file = std::fopen(name, mode);

			if (file == nullptr)
			{
#ifdef SCL_USE_ERROR
				error::set_fopen(name, mode);
				error::set_file_info(__FILE__, __LINE__);
#else
				throw new fopen_error(name, mode);
#endif
			}

			return file;
		}

		static inline long safe_ftell(FILE *stream)
		{
			long value;

			value = std::ftell(stream);
			if (value == -1)
			{
#ifdef SCL_USE_ERROR
				error::set_ftell();
				error::set_file_info(__FILE__, __LINE__);
#else
				throw new ftell_error();
#endif
			}

			return value;
		}

		static inline void safe_fseek(FILE *stream, long int offset, int origin)
		{
			if (fseek(stream, offset, origin))
			{
#ifdef SCL_USE_ERROR
				error::set_fseek();
				error::set_file_info(__FILE__, __LINE__);
#else
				throw new fseek_error();
#endif
			}
		}

		static inline long get_file_size(FILE *stream)
		{
			long size;

			safe_fseek(stream, 0, SEEK_END);

#ifdef SCL_USE_ERROR
			if (error::check()) return 0;
#endif

			size = safe_ftell(stream);

#ifdef SCL_USE_ERROR
			if (error::check()) return 0;
#endif

			safe_fseek(stream, 0, SEEK_SET);
			
#ifdef SCL_USE_ERROR
			if (error::check()) return 0;
#endif

			return size;
		}

		static inline size_t safe_fread(void *pntr, size_t size, FILE *stream)
		{
			size_t read_number;

			read_number = fread(pntr, 1, size, stream);
			if (read_number != size)
			{
#ifdef SCL_USE_ERROR
				error::set_fread(stream, size);
				error::set_file_info(__FILE__, __LINE__);
#else
				throw new fread_error(stream, size);
#endif // SCL_USE_ERROR

			}

			return read_number;
		}

		static inline size_t safe_write(void *pntr, size_t size, FILE *stream)
		{
			size_t write_number;

			write_number = fwrite(pntr, 1, size, stream);
			if (write_number != size)
			{
#ifdef SCL_USE_ERROR
				error::set_fwrite(stream, size);
				error::set_file_info(__FILE__, __LINE__);
#else
				throw new fwrite_error(stream, size);
#endif // SCL_USE_ERROR
			}

			return write_number;
		}

		static inline void print_sep(std::string sep, std::string end = "\n", uint64_t repeat = 80)
		{
			for (uint64_t step = 0; step < repeat; step += 1)
				std::cout << std::string(sep);
			std::cout << std::string(end);
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