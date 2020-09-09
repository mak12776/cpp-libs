#pragma once

#include "pch.h"

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <stdarg.h>
#include <inttypes.h>
#include <errno.h>

#include "err.h"
#include "math.h"
#include "clibs.h"


namespace scl
{
	namespace err
	{
		num_t num = err::NO_ERROR;
		size_t info_array_index = 0;
		info_t info_array[info_array_size];

		// function definitions

		inline int printf(const char *fmt, ...)
		{
			const char *str;
			va_list ap;
			int total, ret;

			total = 0;
			if (fmt == nullptr)
			{
				ret = std::printf("error: %s\n", to_string(num));
				if (ret == -1)
					return -1;

				if (math::add(total, ret, total))
					return -1;
			}
			else
			{
				va_start(ap, fmt);
				ret = cl::vasprintf(&str, fmt, ap);
				va_end(ap);

				if (ret == -1)
					return -1;

				ret = std::printf("error: %s: %s\n", to_string(num), str);
				if (ret == -1)
					return -1;

				if (math::add(total, ret, total))
					return -1;
			}

			// printf strerror if necessary
			if (num == FOPEN)
			{
				ret = std::printf("errno: %s\n", strerror(errno));
				if (ret == -1)
					return -1;
			}

			// printf file info array
			ret = std::printf("call trace back:\n");
			if (ret == -1)
				return -1;

			for (size_t index = 0; index < info_array_index; index += 1)
			{
				ret = std::printf("  %s:%" PRIuLINE ": %s\n",
					info_array[index].file_name,
					info_array[index].line_number,
					info_array[index].function_name);
				if (ret == -1)
					return -1;

				if (math::add(total, ret, total))
					return -1;
			}
		}

		inline void set(num_t errnum) { num = errnum; }
		inline void clear()
		{
			num = num_t::NO_ERROR;
			info_array_index = 0;
		}
		inline bool check() { return num != num_t::NO_ERROR; }
		inline bool success() { return num == num_t::NO_ERROR; }

		inline const char *string() { return to_string(num); }

		inline void push_file_info(const char *file_name, uint64_t line_number, const char *function_name, const char *function_sig)
		{
			if (info_array_index != info_array_size)
			{
				info_array[info_array_index].file_name = file_name;
				info_array[info_array_index].line_number = line_number;
				info_array[info_array_index].function_sig = function_sig;
				info_array[info_array_index].function_name = function_name;
				info_array_index += 1;
			}
		}
	}
}

