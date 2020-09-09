#pragma once

#include <cstdarg>
#include <cstdio>

#include "math.h"

namespace cl
{
	const int PRINTF_ERROR = -1;

#ifdef _MSC_VER
	static inline int vasprintf(char **strp, const char *fmt, va_list ap)
	{
		using namespace scl;

		va_list ap_copy;
		char *str;
		size_t size;
		int ret;

		// _vsprintf will not return negative values, or failed.
		va_copy(ap_copy, ap);
		ret = _vscprintf(fmt, ap_copy);

		if (math::add((size_t)ret, (size_t)1, size))
			return PRINTF_ERROR;

		str = (char *)malloc(size);
		if (str == nullptr)
			return PRINTF_ERROR;

		ret = vsprintf(str, fmt, ap);
		if (ret < 0)
		{
			free(str);
			return PRINTF_ERROR;
		}

		*strp = str;
		return ret;
	}

	static inline int asprintf(char **strp, const char *fmt, ...)
	{
		using namespace scl;

		va_list ap;
		int ret;

		va_start(ap, fmt);
		ret = vasprintf(strp, fmt, ap);
		va_end(ap);

		return ret;
	}
#endif

	static inline int vfprintf_ln(FILE *stream, const char *fmt, va_list ap)
	{
		char *str;
		int ret;

		ret = vasprintf(&str, fmt, ap);
		if (ret < 0)
			return PRINTF_ERROR;

		ret = fprintf(stream, "%s\n", str);
		free((void*)str);

		return ret;
	}

	static inline int fprintf_ln(FILE *stream,const char *fmt, ...)
	{
		int ret;
		va_list ap;
		
		va_start(ap, fmt);
		ret = vfprintf_ln(stream, fmt, ap);
		va_end(ap);

		return ret;
	}

	static inline int printf_ln(const char *fmt, ...)
	{
		int ret;
		va_list ap;

		va_start(ap, fmt);
		ret = vfprintf_ln(stdout, fmt, ap);
		va_end(ap);

		return ret;
	}

	template <typename value_type>
	static inline int print_size_of(FILE *stream = stdout)
	{
		return fprintf(stream, "sizeof %s: %zu\n", typeid(value_type).name(),
			sizeof(value_type));
	}

	template <typename value_type>
	static inline int print_address_of(value_type *value, const char *name = nullptr, FILE *stream = stdout)
	{
		if (name == nullptr) name = "...";
		return fprintf(stream, "address of %s: %zu\n", name, value);
	}

}
