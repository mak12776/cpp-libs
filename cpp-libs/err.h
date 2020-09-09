#pragma once

#include <cinttypes>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <stdarg.h>
#include <inttypes.h>
#include <errno.h>

#include "err.h"
#include "math.h"
#include "clib.h"

namespace scl
{
	namespace err
	{
#define PRIuLINE PRIu64
		typedef uint64_t line_t;

		enum num_t : uint8_t
		{
			SUCCESS = 0,
			INVALID_ARGUMENT,
			WIN_ERROR, PRINTF,
			MALLOC, NEW,
			INT_OVERFLOW, FLOAT_OVERFLOW,
			FOPEN, FTELL, FSEEK,
			FREAD, FWRITE,
			FERROR, FEOF,
			UNDEFINED_BEHAVIOR,
			INVALID_FILE_STRUCTURE,
		};

		static inline const char *to_string(enum num_t num)
		{
			switch (num)
			{
			case SUCCESS: return "SUCCESS";
			case INVALID_ARGUMENT: return "INVALID_ARGUMENT";
			case WIN_ERROR: return "WIN_ERROR";
			case PRINTF: return "PRINTF";
			case MALLOC: return "MALLOC";
			case NEW: return "NEW";
			case INT_OVERFLOW: return "INT_OVERFLOW";
			case FLOAT_OVERFLOW: return "FLOAT_OVERFLOW";
			case FOPEN: return "FOPEN";
			case FTELL: return "FTELL";
			case FSEEK: return "FSEEK";
			case FREAD: return "FREAD";
			case FWRITE: return "FWRITE";
			case FERROR: return "FERROR";
			case FEOF: return "FEOF";
			case UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case INVALID_FILE_STRUCTURE: return "INVALID_FILE_STRUCTURE";
			default: return "UKNOWN";
			}
		}

		num_t num = SUCCESS;
		size_t info_array_index = 0; 
		constexpr size_t info_array_size = 1024;

#pragma pack(push, 1)
		struct info_t
		{
			const char *file_name;
			line_t line_number;
			const char *function_name;
		} info_array[info_array_size];

		struct error_t
		{
			num_t num;

			size_t array_index;
			info_t info_array[info_array_size];
		};
#pragma pack(pop)

		FILE *log_file = stderr;

		static inline int print_status()
		{
			return std::fprintf(log_file, "%s: %s\n", to_string(num), strerror(errno));
		}

		static inline int printf_status(const char *fmt, ...)
		{
			va_list ap;
			char *str;
			int ret;

			va_start(ap, fmt);
			ret = cl::vasprintf(&str, fmt, ap);
			va_end(ap);

			if (ret < 0)
				return cl::PRINTF_ERROR;

			ret = std::fprintf(log_file, "%s: %s: %s\n", to_string(num), str, strerror(errno));
			free(str);

			return ret;
		}

		static inline int print_traceback()
		{
			int ret, total;

			// printf file info array
			ret = std::fprintf(log_file, "Traceback (most recent call last):\n");
			if (ret < 0)
				return cl::PRINTF_ERROR;

			total = ret;
			for (size_t index = info_array_index - 1; index != SIZE_MAX; index -= 1)
			{
				ret = std::fprintf(log_file, "  %s:%" PRIuLINE ": %s\n",
					info_array[index].file_name,
					info_array[index].line_number,
					info_array[index].function_name);

				if (ret < 0)
					return cl::PRINTF_ERROR;

				if (math::add(total, ret, total))
					return cl::PRINTF_ERROR;
			}

			return total;
		}

		static inline int printf(const char *fmt, ...)
		{
			int total, ret;
			va_list ap;
			char *str;

			if (fmt == nullptr)
				ret = err::print_status();
			else
			{
				va_start(ap, fmt);
				ret = cl::vasprintf(&str, fmt, ap);
				va_end(ap);

				if (ret < 0)
					return cl::PRINTF_ERROR;

				ret = std::fprintf(log_file, "%s: %s: %s\n", to_string(num), str, strerror(errno));
				free(str);
			}

			if (ret < 0)
				return cl::PRINTF_ERROR;

			total = print_traceback();
			if (total < 0)
				return cl::PRINTF_ERROR;

			if (math::add(total, ret, total))
				return cl::PRINTF_ERROR;

			return total;
		}

		static inline void set(num_t errnum) { num = errnum; }
		static inline void clear()
		{
			num = num_t::SUCCESS;
			info_array_index = 0;
		}
		static inline bool check() { return num != num_t::SUCCESS; }
		static inline bool success() { return num == num_t::SUCCESS; }

		inline const char *string() { return to_string(num); }

		inline void push_file_info(const char *file_name, uint64_t line_number, const char *function_name)
		{
			if (info_array_index != info_array_size)
			{
				info_array[info_array_index].file_name = file_name;
				info_array[info_array_index].line_number = line_number;
				info_array[info_array_index].function_name = function_name;
				info_array_index += 1;
			}
		}
	}
}

