#pragma once

#include <stdio.h>
#include <stdarg.h>

namespace scl
{
	namespace log
	{
		struct logger_t
		{
			FILE *log_file;

			inline int printf(const char *fmt, ...)
			{
				va_list ap;
				int ret;

				va_start(ap, fmt);
				ret = vfprintf(log_file, fmt, ap);
				va_end(ap);

				return ret;
			}
		};

		logger_t default_logger{ stdout };

		FILE *log_file;

		static inline bool init_log_file()
		{
			// TODO: write this function
		}


		static inline int printf(const char *fmt, ...)
		{
			va_list ap;
			int ret;

			va_start(ap, fmt);
			ret = std::vfprintf(log_file, fmt, ap);
			va_end(ap);

			return ret;
		}

	}
}