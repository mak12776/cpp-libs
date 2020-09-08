#pragma once

#include <stdarg.h>
#include <stdio.h>

#include "math.h"

// c printf functions

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