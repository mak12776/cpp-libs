#pragma once

#include <cstdarg>
#include <cstdio>

#include "err.h"

namespace scl
{
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

		return (size_t)ret;
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

		return (size_t)ret;
	}

}