#pragma once

namespace scl
{
	namespace err
	{
		enum num_t : uint8_t
		{
			SUCCESS = 0,

			// invalid argument
			INVALID_ARGUMENT,

			// malloc, new operator
			NO_MEMORY, NEW,

			// overflows
			INT_OVERFLOW, FLOAT_OVERFLOW,

			// normal function
			OPEN, STAT, READ, WRITE,

			// file functions
			FOPEN, FTELL, FSEEK, FREAD, FWRITE,

			// other errors
			WIN_ERROR, PRINTF, UNDEFINED_BEHAVIOR,
			INVALID_FILE_STRUCTURE,
		};

		static inline const char *to_string(enum num_t num)
		{
			switch (num)
			{
			case SUCCESS: return "SUCCESS";
			case INVALID_ARGUMENT: return "INVALID_ARGUMENT";

			case NO_MEMORY: return "NO_MEMORY";
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


#define PRIuLINE PRIu64
		typedef uint64_t line_t;

		struct info_t
		{
			const char *file_name;
			line_t line_number;
			const char *function_name;
		};

		template <size_t _array_size>
		struct err_t
		{
			num_t num = num_t::SUCCESS;
			size_t array_index = 0;

			FILE *log_file = stderr;
			info_t info_array[_array_size];

			// functions

			inline constexpr num_t &get_num() { return num; }
			
			inline void set(num_t errnum) { num = errnum; }
			inline bool test(num_t errnum) { return num == errnum; }
			inline void clear() { num = num_t::SUCCESS; array_index = 0; }
			inline void clear_if(num_t errnum) { if (num == errnum) clear(); }

			inline bool check() { return num != num_t::SUCCESS; }
			inline const char *string() { return to_string(num); }

			inline void push_file_info(const char *file_name, uint64_t line_number,
				const char *function_name)
			{
				if (array_index != _array_size)
				{
					info_array[array_index].file_name = file_name;
					info_array[array_index].line_number = line_number;
					info_array[array_index].function_name = function_name;
					array_index += 1;
				}
			}

			inline bool check_push_file_info(const char *file_name, uint64_t line_number,
				const char *function_name)
			{
				if (check())
				{
					push_file_info(file_name, line_number, function_name);
					return true;
				}
				return false;
			}

			inline size_t write_traceback(FILE *file = nullptr)
			{
				size_t write_number;
				size_t total = 0;

				file = (file == nullptr) ? log_file : file;
				cfmt::write(file, write_number,
					"Traceback (most recent call last):\n");

				for (size_t index = array_index - 1; index != SIZE_MAX; index -= 1)
				{
					cfmt::write(file, write_number, " %s: %" PRIuLINE ": %s\n", 
						info_array[index].file_name,
						info_array[index].line_number,
						info_array[index].function_name);

					total += write_number;
				}

				return total;
			}

			inline size_t write(FILE *file = nullptr)
			{
				size_t write_number;
				file = (file == nullptr) ? log_file : file;

				switch (num)
				{
				case scl::err::SUCCESS:
					return 0;

				case scl::err::INVALID_ARGUMENT:
				case scl::err::NO_MEMORY:
				case scl::err::NEW:
				case scl::err::INT_OVERFLOW:
				case scl::err::FLOAT_OVERFLOW:

				case scl::err::OPEN:
				case scl::err::STAT:
				case scl::err::READ:
				case scl::err::WRITE:

				case scl::err::WIN_ERROR:
				case scl::err::PRINTF:
				case scl::err::UNDEFINED_BEHAVIOR:
				case scl::err::INVALID_FILE_STRUCTURE:
					cfmt::write(file, write_number, "error: %s\n", 
						to_string(num));
					break;

				case scl::err::FOPEN:
				case scl::err::FTELL:
				case scl::err::FSEEK:
				case scl::err::FREAD:
				case scl::err::FWRITE:
					cfmt::write(file, write_number, "error: %s: %s\n", 
						to_string(num), strerror(errno));
					break;
				
				default:
					cfmt::write(file, write_number, "error: unknown error!\n", to_string(num));
				}	

				write_number += write_traceback(file);
				return write_number;
			}

			inline void write_exit(FILE *file = nullptr)
			{
				write(file);
				exit(1);
			}
		};
#pragma pack(pop)


		// default err
		constexpr size_t default_array_size = 4096;

		typedef err_t<default_array_size> default_err_t;
		default_err_t default_err;

		// global functions

		static inline void set(num_t errnum) { default_err.set(errnum); }
		static inline bool test(num_t errnum) { return default_err.test(errnum); }
		static inline void clear() { default_err.clear(); }
		static inline void clear_if(num_t num) { default_err.clear_if(num); }

		static inline bool check() { return default_err.check(); }
		inline const char *string() { return  default_err.string(); }

		inline void push_file_info(const char *file_name, line_t line_number,
			const char *function_name)
		{
			default_err.push_file_info(file_name, line_number, function_name);
		}

		inline bool check_push_file_info(const char *file_name, uint64_t line_number,
			const char *function_name)
		{
			return default_err.check_push_file_info(file_name, line_number, function_name);
		}

		static inline size_t write_traceback(FILE *file = nullptr) { return default_err.write_traceback(file); }
		static inline size_t write(FILE *file = nullptr) { return default_err.write(file); }
		static inline void write_exit(FILE *file = nullptr) { default_err.write_exit(file); }

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

