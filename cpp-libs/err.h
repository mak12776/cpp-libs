#pragma once

#include <cinttypes>

namespace scl
{
	namespace err
	{

		// types, enumarations

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

		constexpr size_t info_array_size = 1024;

#pragma pack(push, 1)
		struct info_t
		{
			const char *file_name;
			line_t line_number;
			const char *function_name;
			const char *function_sig;
		};

		struct error_t
		{
			num_t num;

			size_t array_index;
			info_t info_array[info_array_size];
		};
#pragma pack(pop)

		extern num_t num;
		extern size_t info_array_index;
		extern info_t info_array[info_array_size];

		// functions

		extern inline int printf(const char *fmt, ...);

		extern inline void set(num_t errnum);
		extern inline void clear();
		extern inline bool check();
		extern inline bool success();

		extern inline const char *string();

		extern inline void push_file_info(const char *file_name, uint64_t line_number, const char *function_name, const char *function_sig);
	}
}

