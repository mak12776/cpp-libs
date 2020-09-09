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
			NO_ERROR = 0,
			INVALID_ARGUMENT,
			PRINTF, MALLOC, NEW,
			WIN_ERROR,
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
			case NO_ERROR: return "NO_ERROR";
			case INVALID_ARGUMENT: return "INVALID_ARGUMENT";
			case PRINTF: return "PRINTF";
			case MALLOC: return "MALLOC";
			case NEW: return "NEW";
			case WIN_ERROR: return "WIN_ERROR";
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

		num_t num = err::NO_ERROR;
		size_t info_array_index = 0; 
		constexpr size_t info_array_size = 1024;

#pragma pack(push, 1)
		struct info_t
		{
			const char *file_name;
			line_t line_number;
			const char *function_name;
		};

		struct error_t
		{
			num_t num;

			size_t array_index;
			info_t info_array[info_array_size];
		};
#pragma pack(pop)

		info_t info_array[info_array_size];

		static inline int printf(const char *fmt, ...)
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

		static inline void set(num_t errnum) { num = errnum; }
		static inline void clear()
		{
			num = num_t::NO_ERROR;
			info_array_index = 0;
		}
		static inline bool check() { return num != num_t::NO_ERROR; }
		static inline bool success() { return num == num_t::NO_ERROR; }

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

