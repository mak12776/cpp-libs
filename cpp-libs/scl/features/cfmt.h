#pragma once

namespace scl
{
	namespace cfmt
	{
		// --- low format ---
		// using stdio printf family functions with better function names.
		// doesn't support sized strings.

		static inline size_t cast_int(int value)
		{
#if INT_MAX > SIZE_MAX
#error INT_MAX is to big.
#endif		
			return (size_t)value;
		}

		// get_len

		static inline bool get_len_valist(size_t &result_len, const char *fmt, va_list list)
		{
			int ret;

			ret = _vscprintf_p(fmt, list);
			if (ret < 0)
				return true;

			result_len = cast_int(ret);
			return false;
		}

		static inline size_t safe_get_len_valist(const char *fmt, va_list list)
		{
			size_t len = 0;
			if (get_len_valist(len, fmt, list))
			{
				err::set(err::PRINTF);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
			return len;
		}

		// format

		static inline bool format_valist(char *buffer, size_t buffer_size, const char *fmt, va_list list)
		{
			int ret;

			ret = _vsprintf_p(buffer, buffer_size, fmt, list);
			if (ret < 0)
				return true;

			if (cast_int(ret) + 1 != buffer_size)
				return true;

			return false;
		}

		static inline void safe_format_valist(char *buffer, size_t buffer_size, const char *fmt, va_list list)
		{
			if (format_valist(buffer, buffer_size, fmt, list))
			{
				err::set(err::PRINTF);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		// malloc format

		static inline bool malloc_format_valist(char *&buffer, size_t &buffer_size, const char *fmt, va_list list)
		{
			size_t len;
			va_list list_copy;

			va_copy(list_copy, list);	 
			if (get_len_valist(len, fmt, list_copy))
				return true;

			if (len == SIZE_MAX)
				return true;
			len += 1;

			buffer = (char *)malloc(len);
			if (buffer == nullptr)
				return true;
			buffer_size = len;

			if (format_valist(buffer, buffer_size, fmt, list))
			{
				free(buffer);
				return true;
			}

			return false;
		}

		// safe malloc format

		static inline void safe_malloc_format_valist(char *&buffer, size_t &buffer_size, const char *fmt, va_list list)
		{
			size_t len;
			va_list list_copy;

			va_copy(list_copy, list);
			if (get_len_valist(len, fmt, list_copy))
			{
				err::set(err::PRINTF);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			if (len == SIZE_MAX)
			{
				err::set(err::INT_OVERFLOW);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			len += 1;

			buffer = (char *)malloc(len);
			if (buffer == nullptr)
			{
				err::set(err::NO_MEMORY);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			buffer_size = len;

			if (format_valist(buffer, buffer_size, fmt, list))
			{
				err::set(err::PRINTF);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

				free(buffer);
				return;
			}
		}

		static inline void safe_malloc_format(char *&buffer, size_t &buffer_size, const char *fmt, ...)
		{
			va_list list;

			va_start(list, fmt);
			safe_malloc_format_valist(buffer, buffer_size, fmt, list);
			va_end(list);

			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}

		// write

		static inline bool write_valist(FILE *file, size_t &write_number, const char *fmt, va_list list)
		{
			char *pntr;
			size_t size;

			if (malloc_format_valist(pntr, size, fmt, list))
			{
				write_number = 0;
				return true;
			}

			write_number = fwrite(pntr, 1, size, file);
			free(pntr);
			return write_number == size;
		}

		static inline bool write(FILE *file, size_t &write_number, const char *fmt, ...)
		{
			va_list list;
			bool result;

			va_start(list, fmt);
			result = write_valist(file, write_number, fmt, list);
			va_end(list);

			return result;
		}

		// safe write

		static inline size_t safe_write_valist(FILE *file, const char *fmt, va_list list)
		{
			char *pntr;
			size_t size;
			size_t write_number;

			safe_malloc_format_valist(pntr, size, fmt, list);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return 0;

			write_number = fwrite(pntr, 1, size, file);
			free(pntr);

			if (write_number != size)
			{
				err::set(err::FWRITE);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
			return write_number;
		}

		static inline size_t safe_write(FILE *file, const char *fmt, ...)
		{
			va_list list;
			size_t write_number;

			va_start(list, fmt);
			write_number = safe_write_valist(file, fmt, list);
			va_end(list);

			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return write_number;
		}

		// other print functions

		template <typename value_type>
		static inline int print_size_of(FILE *stream)
		{
			return fprintf(stream, "sizeof %s: %zu\n", typeid(value_type).name(),
				sizeof(value_type));
		}

		template <typename value_type>
		static inline int print_address_of(value_type *value, const char *name, FILE *stream)
		{
			name = (name == nullptr) ? "..." : name;
			return fprintf(stream, "address of %s: %zu\n", name, value);
		}

		size_t DEFAULT_WIDTH = 80;

		size_t print_separator(size_t width, ubyte character, FILE *stream)
		{
			ubyte *buffer;
			size_t write_number;

			width = (width == 0) ? DEFAULT_WIDTH : width;

			buffer = (ubyte *)mem::safe_malloc(width + 1);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return 0;

			for (size_t length = 0; length < width; length += 1)
				buffer[length] = character;
			buffer[width++] = '\n';

			write_number = fwrite(buffer, 1, width, stream);

			free(buffer);
			return write_number;
		}
	}
}
