#pragma once

#include <stdarg.h>
#include <stdio.h>

// c printf functions

namespace cl
{
	const int INT_ERROR = -1;

#ifdef _MSC_VER
	extern int vasprintf(const char **strp, const char *fmt, va_list ap);
	extern int asprintf(const char **strp, const char *fmt, ...);
#endif

	extern int printf_ln(const char *fmt, ...);

	template <typename value_type>
	extern int print_size_of(FILE *stream = stdout);

	template <typename value_type>
	extern int print_address_of(value_type *value, const char *name = nullptr, FILE *stream = stdout);
}
