#pragma once

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "scl/all.h"

namespace cl
{
	const int PRINTF_ERROR = -1;

#ifdef _MSC_VER
	static inline int vasprintf(char **strp, const char *fmt, va_list ap)
	{
		using namespace scl;

		va_list ap_copy;
		size_t size;
		char *str;
		int ret;

		// _vsprintf will not return negative values, unless failed.
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

#if 0
	static inline int vfprintff(FILE *stream, const char *line, char *fmt, va_list ap)
	{
		va_list ap_copy;
		char *str;
		int ret;

		ret = vasprintf(&str, fmt, ap);
		if (ret < 0)
			return PRINTF_ERROR;

		ret = printf(line, str);
		free(str);

		return ret;
	}
#endif

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

	static inline size_t safe_fprintf(FILE *stream, const char *fmt, ...)
	{
		va_list ap;
		int ret;

		va_start(ap, fmt);
		ret = vfprintf(stream, fmt, ap);
		va_end(ap);

		if (ret < 0)
		{
			scl::err::set(scl::err::PRINTF);
			scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

			return 0;
		}

		return static_cast<size_t>(ret);
	}

	static inline size_t safe_printf(const char *fmt, ...)
	{
		va_list ap;
		int ret;

		va_start(ap, fmt);
		ret = vprintf(fmt, ap);
		va_end(ap);

		if (ret < 0)
		{
			scl::err::set(scl::err::PRINTF);
			scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

			return 0;
		}

		return static_cast<size_t>(ret);
	}
}
