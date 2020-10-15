#pragma once

namespace scl
{
	namespace log
	{
		struct log_t
		{
			FILE *log_file;

			inline size_t write(void *pntr, size_t size)
			{ return fwrite(pntr, 1, size, log_file); }

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

		log_t default_log{ stdout };
		log_t global_log = default_log;


		// functions

		static inline int printf(const char *fmt, ...)
		{
			va_list ap;
			int ret;

			va_start(ap, fmt);
			ret = std::vfprintf(default_log.log_file, fmt, ap);
			va_end(ap);

			return ret;
		}

	}
}