#pragma once

#include <cstdint>

namespace scl
{
	namespace err
	{
		enum num_t : uint8_t
		{
			NO_ERROR,
			MALLOC, NEW,
			INVALID_ARGUMENT,
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
			case MALLOC: return "MALLOC";
			case NEW: return "NEW";
			case INVALID_ARGUMENT: return "INVALID_ARGUMENT";
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

		num_t num;

		constexpr size_t info_array_size = 1024;
		size_t info_array_index = 0;

		struct
		{
			const char *file_name;
			uint64_t line_number;
			const char *function_sig;
		} info_array[info_array_size];

		static inline int printf()
		{

		}

		static inline void set(num_t errnum) { num = errnum; }
		static inline void clear() 
		{
			num = num_t::NO_ERROR; 
			info_array_index = 0;
		}
		static inline bool check() { return num != num_t::NO_ERROR; }
		static inline bool success() { return num == num_t::NO_ERROR; }

		static inline const char *string() { return to_string(num); }

		static inline void push_file_info(const char *file_name, uint64_t line_number, const char *function_sig)
		{
			if (info_array_index != info_array_size)
			{
				info_array[info_array_index].file_name = file_name;
				info_array[info_array_index].line_number = line_number;
				info_array[info_array_index].function_sig = function_sig;
				info_array_index += 1;
			}
		}
	}
}

