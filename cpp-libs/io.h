#pragma once

#include <cstdio>
#include <stdarg.h>

#include "clib.h"
#include "err.h"
#include "mem.h"

namespace scl
{
	namespace io
	{
		static inline FILE *safe_fopen(const char *name, const char *mode)
		{
			FILE *file = std::fopen(name, mode);

			if (file == nullptr)
			{
				err::set(err::FOPEN);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}

			return file;
		}

		static inline long safe_ftell(FILE *stream)
		{
			long value;

			value = std::ftell(stream);
			if (value == -1)
			{
				err::set(err::FTELL);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}

			return value;
		}

		static inline void safe_fseek(FILE *stream, long offset, int origin)
		{
			if (fseek(stream, offset, origin))
			{
				err::set(err::FSEEK);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		static inline long get_file_size(FILE *stream)
		{
			long size;

			safe_fseek(stream, 0, SEEK_END);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			size = safe_ftell(stream);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			safe_fseek(stream, 0, SEEK_SET);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			return size;
		}

		static inline size_t safe_fread(void *pntr, size_t size, FILE *stream)
		{
			size_t read_number;

			read_number = fread(pntr, 1, size, stream);
			if (read_number != size)
			{
				err::set(err::FREAD);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}

			return read_number;
		}

		template <typename data_type>
		static inline size_t safe_fread_data(data_type &data, FILE *file)
		{
			return safe_fread(&data, sizeof(data_type), file);
		}

		static inline size_t safe_fwrite(void *pntr, size_t size, FILE *stream)
		{
			size_t write_number;

			write_number = fwrite(pntr, 1, size, stream);
			if (write_number != size)
			{
				err::set(err::FWRITE);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}

			return write_number;
		}

		template<typename data_type>
		static inline size_t safe_fwrite_data(data_type data, FILE *stream)
		{
			return safe_fwrite(&data, sizeof(data_type), stream);
		}

		static inline size_t fread_all(void *pntr, size_t size, FILE *stream)
		{
			size_t read_number;

			read_number = fread(pntr, 1, size, stream);
			while (read_number != size)
			{
				if (ferror(stream))
				{
					err::set(err::FERROR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else if (feof(stream))
				{
					err::set(err::FEOF);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else
				{
					err::set(err::UNDEFINED_BEHAVIOR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				read_number += fread((ubyte *)pntr + read_number, 1, size - read_number, stream);
			}
			return read_number;
		}

		static inline size_t fwrite_all(void *pntr, size_t size, FILE *stream)
		{
			size_t write_number;

			write_number = fwrite(pntr, 1, size, stream);
			while (write_number != size)
			{
				if (ferror(stream))
				{
					err::set(err::FERROR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else
				{
					err::set(err::UNDEFINED_BEHAVIOR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				write_number += fwrite((ubyte *)pntr + write_number, 1, size - write_number, stream);
			}
			return write_number;
		}

		static inline void read_file(FILE *file, void **pntr, size_t *size)
		{
			long file_long_size;
			size_t file_size;

			file_long_size = get_file_size(file);

#if ULONG_MAX < SIZE_MAX
			file_size = (unsigned long)file_long_size;
#else
#error unsigned long is too big.
#endif

			(*pntr) = mem::safe_malloc(file_size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			(*size) = safe_fread(*pntr, file_size, file);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				mem::free(*pntr);
				return;
			}
		}

		static inline void read_file_name(const char *name, void **pntr, size_t *size)
		{
			FILE *file = safe_fopen(name, "rb");
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			read_file(file, pntr, size);
			std::fclose(file);
		}

		// normal print, fomrated print tools

		size_t DEFAULT_WIDTH = 80;
		void set_default_width(size_t width) { DEFAULT_WIDTH = width; }
		void reset_default_width() { DEFAULT_WIDTH = 80; }

		size_t print_separator(size_t width = 0, ubyte character = '-', FILE *stream = stdout)
		{
			ubyte *buffer;
			size_t write_number;

			if (width == 0) width = DEFAULT_WIDTH;

			buffer = mem::new_array<ubyte>(width + 1);

			if (scl::err::check())
				return 0;

			for (size_t index = 0; index < width; index += 1)
				buffer[index] = character;
			buffer[width++] = '\n';

			write_number = fwrite(buffer, 1, width, stream);
			mem::free(buffer);

			return write_number;
		}
	}
}