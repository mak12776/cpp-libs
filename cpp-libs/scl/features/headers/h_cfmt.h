#pragma once

namespace scl
{
	namespace cfmt
	{
		static inline size_t cast_int(int value);
		static inline bool get_len_valist(size_t &result_len, const char *fmt, va_list list);
		static inline size_t safe_get_len_valist(const char *fmt, va_list list);

		static inline bool format_valist(char *buffer, size_t buffer_size, const char *fmt, va_list list);
		static inline void safe_format_valist(char *buffer, size_t buffer_size, const char *fmt, va_list list);

		static inline bool malloc_format_valist(char *&buffer, size_t &buffer_pntr, const char *fmt, va_list list);

		static inline void safe_malloc_format_valist(char *&buffer, size_t &buffer_size, const char *fmt, va_list list);
		static inline void safe_malloc_format(char *&buffer, size_t &buffer_size, const char *fmt, ...);
		
		static inline size_t check_writeln_valist(FILE *file, bool &err, const char *fmt, va_list list);
		static inline size_t check_writeln(FILE *file, bool &err, const char *fmt, ...);

		static inline size_t writeln_valist(FILE *file, const char *fmt, va_list list);
		static inline size_t writeln(FILE *file, const char *fmt, ...);

		static inline size_t safe_write_valist(FILE *file, const char *fmt, va_list list);
		static inline size_t safe_write(FILE *file, const char *fmt, ...);

		// other print functions

		template <typename value_type>
		static inline int print_size_of(FILE *stream = stdout);
		template <typename value_type>
		static inline int print_address_of(value_type *value, const char *name = nullptr, FILE *stream = stdout);
		size_t print_separator(size_t width = 0, ubyte character = '-', FILE *stream = stdout);
	}
}
