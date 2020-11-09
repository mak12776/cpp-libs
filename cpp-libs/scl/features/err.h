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

		struct err_t
		{
			num_t num = num_t::SUCCESS;
			size_t array_index = 0;

			size_t array_size;
			info_t *info_array;

			// functions
			
			void set(num_t errnum) { num = errnum; }
			void clear() { num = num_t::SUCCESS; array_index = 0; }
			void clear_if(num_t errnum) { if (num == errnum) clear(); }

			bool check() { return num != num_t::SUCCESS; }
			bool test(num_t errnum) { return num == errnum; }

			const char *string() { return to_string(num); }

			void pop_file_info() { array_index -= 1; }

			void push_file_info(const char *file, line_t line, const char *function)
			{
				if (array_index != array_size)
				{
					info_array[array_index].file_name = file;
					info_array[array_index].line_number = line;
					info_array[array_index].function_name = function;
					array_index += 1;
				}
			}

			bool check_push_file_info(const char *file, line_t line, const char *function)
			{
				if (check())
				{
					push_file_info(file, line, function);
					return true;
				}
				return false;
			}

			size_t print_traceback(FILE *file = nullptr)
			{
				size_t total;
				file = (file == nullptr) ? stderr : file;

				total = cfmt::write(file,
					"Traceback (most recent call last):\n");

				for (size_t index = array_index - 1; index != SIZE_MAX; index -= 1)
				{
					total += cfmt::write(file, " %s: %" PRIuLINE ": %s\n", 
						info_array[index].file_name,
						info_array[index].line_number,
						info_array[index].function_name);
				}

				return total;
			}

			size_t print(FILE *file = nullptr)
			{
				file = (file == nullptr) ? stderr : file;
				size_t write_number = 0;

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
					write_number += 
						cfmt::write(file, "error: %s\n", to_string(num));
					break;

				case scl::err::FOPEN:
				case scl::err::FTELL:
				case scl::err::FSEEK:
				case scl::err::FREAD:
				case scl::err::FWRITE:
					write_number += 
						cfmt::write(file, "error: %s: %s\n", to_string(num), strerror(errno));
					break;
				
				default:
					write_number +=
						cfmt::write(file, "error: unknown error!\n");
				}	

				write_number += print_traceback(file);
				return write_number;
			}

			inline void check_print_exit(FILE *file = nullptr)
			{
				print(file);
				if (num != SUCCESS) 
					exit(1);
			}
		};
#pragma pack(pop)


		// default err
		constexpr size_t default_array_size = 8192;
		info_t default_array[default_array_size];

		constexpr err_t default_err{ num_t::SUCCESS, 0, default_array_size, default_array };
		err_t global_err = default_err;

		// global functions

		static constexpr inline void set(num_t errnum) { global_err.set(errnum); }
		static constexpr inline void clear() { global_err.clear(); }
		static constexpr inline void clear_if(num_t num) { global_err.clear_if(num); }
		
		static constexpr inline bool check() { return global_err.check(); }
		static constexpr inline bool test(num_t errnum) { return global_err.test(errnum); }

		static constexpr inline const char *string() { return global_err.string(); }

		static constexpr inline void pop_file_info() { global_err.pop_file_info(); }
		static constexpr inline void push_file_info(const char *file, line_t line, const char *function) { global_err.push_file_info(file, line, function); }
		static constexpr inline bool check_push_file_info(const char *file, uint64_t line, const char *function) { return global_err.check_push_file_info(file, line, function); }

		static constexpr inline size_t print_traceback(FILE *file = nullptr) { return global_err.print_traceback(file); }
		static constexpr inline size_t print(FILE *file = nullptr) { return global_err.print(file); }
		static constexpr inline void check_print_exit(FILE *file = nullptr) { global_err.check_print_exit(file); }
	}
}

