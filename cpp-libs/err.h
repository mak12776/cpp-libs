#pragma once

#include <cinttypes>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <stdarg.h>
#include <inttypes.h>
#include <errno.h>

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

			// invalid argument
			INVALID_ARGUMENT,

			// malloc, new operator
			MALLOC, 
			NEW,

			// overflows
			INT_OVERFLOW, 
			FLOAT_OVERFLOW,

			// normal function
			OPEN, 
			STAT,
			READ, 
			WRITE,

			// file functions
			FOPEN, 
			FTELL, 
			FSEEK,
			FREAD, 
			FWRITE,

			// other errors
			WIN_ERROR, 
			PRINTF,
			UNDEFINED_BEHAVIOR,
			INVALID_FILE_STRUCTURE,
		};

		static inline const char *to_string(enum num_t num)
		{
			switch (num)
			{
			case SUCCESS: return "SUCCESS";
			case INVALID_ARGUMENT: return "INVALID_ARGUMENT";

			case MALLOC: return "MALLOC";
			case NEW: return "NEW"; 

			case INT_OVERFLOW: return "INT_OVERFLOW";
			case FLOAT_OVERFLOW: return "FLOAT_OVERFLOW";

			case STAT: return "STAT";
			case OPEN: return "OPEN";
			case READ: return "READ";
			case WRITE: return "WRITE";

			case FOPEN: return "FOPEN";
			case FTELL: return "FTELL";
			case FSEEK: return "FSEEK";
			case FREAD: return "FREAD";
			case FWRITE: return "FWRITE";

			case WIN_ERROR: return "WIN_ERROR";
			case PRINTF: return "PRINTF";

			case UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case INVALID_FILE_STRUCTURE: return "INVALID_FILE_STRUCTURE";
			default: return "UKNOWN";
			}
		}

#pragma pack(push, 1)

		struct info_t
		{
			const char *file_name;
			line_t line_number;
			const char *function_name;
		};

		template <size_t size>
		struct handler_t
		{
			num_t num;
			size_t index;
			info_t info_array[size];

			constexpr size_t array_size() const { return size; }

			inline bool check() { return num != num_t::SUCCESS; }
			inline void set(num_t errnum) { num = errnum; }
			inline void clear() { num = num_t::SUCCESS; index = 0; }
			inline const char *string() { return to_string(num); }

			inline void push_file_info(const char *file_name, uint64_t line_number,
				const char *function_name)
			{
				if (index != size)
				{
					info_array[index].file_name = file_name;
					info_array[index].line_number = line_number;
					info_array[index].function_name = function_name;
					index += 1;
				}
			}
		};
		

#pragma pack(pop)

		constexpr size_t default_array_size = 4096;

		typedef handler_t<default_array_size> default_handler_t;
		default_handler_t default_handler{SUCCESS, 0, {0}, stderr};


		static inline void set(num_t errnum) { default_handler.set(errnum); }
		static inline void clear() { default_handler.clear(); }

		static inline bool check() { return default_handler.check(); }
		inline const char *string() { return  default_handler.string(); }

		inline void push_file_info(const char *file_name, uint64_t line_number,
			const char *function_name)
		{
			default_handler.push_file_info(file_name, line_number, function_name);
		}

#if SCL_ERR_PRINT
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
#endif
	}
}

