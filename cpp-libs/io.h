#pragma once

#include <cstdio>

#include "error.h"
#include "memory.h"

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

		static inline void safe_fseek(FILE *stream, long offset, int origin)
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
		static inline size_t safe_fwrite(void *pntr, size_t size, FILE *stream)
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


		struct buffer
		{
			uint8_t *pntr;
			size_t size;
		};

		static inline void read_file(FILE *file, void **pntr, size_t *size)
		{
			long file_long_size = get_file_size(file);
			size_t file_size;
#ifdef SCL_USE_ERROR
			if (error::check()) return;
#endif

#if ULONG_MAX < SIZE_MAX
			file_size = (unsigned long)file_long_size;
#else
#error unsigned long is too big.
#endif

			(*pntr) = memory::safe_malloc(file_size);
#ifdef SCL_USE_ERROR
			if (error::check()) return;
#endif

			(*size) = safe_fread(*pntr, file_size, file);
#ifdef SCL_USE_ERROR
			if (error::check())
				scl::memory::free(*pntr);
#endif
		}
#if 0
#endif

		static inline void read_file_name(const char *name, void **pntr, size_t *size)
		{
			FILE *file = safe_fopen(name, "rb");
			if (error::check()) return;

			read_file(file, pntr, size);
			std::fclose(file);
		}
	}
}