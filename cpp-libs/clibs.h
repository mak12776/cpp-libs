#pragma once

#include <stdarg.h>
#include <stdio.h>

#include "math.h"

// c printf functions

namespace cl
{
#ifdef _MSC_VER
	int asprintf(const char **strp, const char *fmt, ...)
	{
		using namespace scl;

		va_list ap;
		size_t size;
		int ret;
		char *str;

		va_start(ap, fmt);
		ret = _vscprintf(fmt, ap);
		va_end(ap);

		if (ret == -1)
			return -1;

		if (math::add((size_t)ret, (size_t)1, size))
			return -1;

		str = (char *)malloc(size);
		if (!str)
			return -1;

		va_start(ap, fmt);
		ret = vsprintf(str, fmt, ap);
		va_end(ap);

		if (ret == -1)
		{
			free(str);
			return -1;
		}

		*strp = str;
		return ret;
	}

	int vasprintf(const char **strp, const char *fmt, va_list ap)
	{
		using namespace scl;

		char *str;
		size_t size;
		int ret;

		ret = _vscprintf(fmt, ap);
		if (ret == -1)
			return -1;

		if (math::add((size_t)ret, (size_t)1, size))
			return -1;

		str = (char *)malloc(size);
		if (!str)
			return -1;

		ret = vsprintf(str, fmt, ap);
		if (ret == -1)
		{
			free(str);
			return -1;
		}

		*strp = str;
		return ret;
	}
#endif

	int printf_ln(const char *fmt, ...)
	{
		const char *pntr;
		int ret;
		va_list ap;

		va_start(ap, fmt);
		ret = vasprintf(&pntr, fmt, ap);
		va_end(ap);

		if (ret == -1)
			return -1;

		ret = printf("%s\n", pntr);
		free((void *)pntr);

		return ret;
	}

	template <typename value_type>
	int print_size_of(FILE *stream = stdout)
	{
		return fprintf(stream, "sizeof %s: %zu\n", typeid(value_type).name(), sizeof(value_type));
	}

	template <typename value_type>
	int print_address_of(value_type *value, const char *name = nullptr, FILE *stream = stdout)
	{
		if (name == nullptr) name = "...";
		return fprintf(stream, "address of %s: %zu\n", name, value);
	}
}
