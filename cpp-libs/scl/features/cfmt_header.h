#pragma once

namespace scl
{
	namespace cfmt
	{
		// get len
		static inline bool get_len_valist(size_t &result_len, const char *fmt, va_list list);
		static inline size_t safe_get_len_valist(const char *fmt, va_list list);

		// format
		static inline bool format_valist(char *buffer, size_t buffer_size, const char *fmt, va_list list);
		static inline void safe_format_valist(char *buffer, size_t buffer_size, const char *fmt, va_list list);

		// malloc format
		static inline bool malloc_format_valist(char *&buffer, size_t &buffer_size, const char *fmt, va_list list);
		static inline void safe_malloc_format_valist(char *&buffer, size_t &buffer_size, const char *fmt, va_list list);
		static inline void safe_malloc_format(char *&buffer, size_t &buffer_size, const char *fmt, ...);

		// write ln
		static inline size_t check_write_ln_valist(bool &err, FILE *file, const char *fmt, va_list list);
		static inline size_t check_write_ln(bool &err, FILE *file, const char *fmt, ...);

		static inline size_t write_ln_valist(FILE *file, const char *fmt, va_list list);
		static inline size_t write_ln(FILE *file, const char *fmt, ...);

		static inline size_t safe_write_ln_valist(FILE *file, const char *fmt, va_list list);
		static inline size_t safe_write_ln(FILE *file, const char *fmt, ...);

		// write
		static inline size_t check_write_valist(bool &err, FILE *file, const char *fmt, va_list list);
		static inline size_t check_write(bool &err, FILE *file, const char *fmt, ...);

		static inline size_t write_valist(FILE *file, const char *fmt, va_list list);
		static inline size_t write(FILE *file, const char *fmt, ...);

		static inline size_t safe_write_valist(FILE *file, const char *fmt, va_list list);
		static inline size_t safe_write(FILE *file, const char *fmt, ...);

		// printf
		static inline size_t printf_valist(const char *fmt, va_list list);
		static inline size_t printf(const char *fmt, ...);

		static inline size_t safe_printf_valist(const char *fmt, va_list list);
		static inline size_t safe_printf(const char *fmt, ...);

		// printf ln
		static inline size_t printf_ln_valist(const char *fmt, va_list list);
		static inline size_t printf_ln(const char *fmt, ...);

		static inline size_t safe_printf_ln_valist(const char *fmt, va_list list);
		static inline size_t safe_printf_ln(const char *fmt, ...);

		// other print fucntions
		template <typename value_type>
		static inline int print_size_of(FILE *stream);

		template <typename value_type>
		static inline int print_address_of(value_type *value, const char *name, FILE *stream);

		size_t print_separator(size_t width, ubyte character, FILE *stream);
	}
}