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
			INT_OVERFLOW, FLOAT_ERROR, TYPE_CAST,

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
			case FLOAT_ERROR: return "FLOAT_ERROR";
			case TYPE_CAST: return "TYPE_CAST";

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
		typedef const char *name_t;

		struct info_t
		{
			name_t file_name;
			line_t line_number;
			name_t function_name;
		};

		struct err_t
		{
			num_t num = num_t::SUCCESS;
			size_t array_index = 0;

			size_t array_size;
			info_t *info_array;

			// functions
			
			inline void set(num_t errnum) { num = errnum; }
			inline void set_push(num_t errnum, name_t file, line_t line, name_t function) { num = errnum; push(file, line, function); }

			inline void clear() { num = num_t::SUCCESS; array_index = 0; }
			inline void clear_if(num_t errnum) { if (num == errnum) clear(); }

			inline bool check() { return num != num_t::SUCCESS; }
			inline bool check_push(name_t file, line_t line, name_t function)
			{
				if (check())
				{
					push(file, line, function);
					return true;
				}
				return false;
			}

			inline bool test(num_t errnum) { return num == errnum; }

			inline const char *string() { return to_string(num); }

			inline void pop() { array_index -= 1; }
			inline void push(name_t file, line_t line, name_t function)
			{
				if (array_index != array_size)
				{
					info_array[array_index].file_name = file;
					info_array[array_index].line_number = line;
					info_array[array_index].function_name = function;
					array_index += 1;
				}
			}

			// print functions

			inline size_t print_traceback(FILE *file = stderr)
			{
				size_t total;

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

			inline size_t print(FILE *file = stderr)
			{
				size_t write_number = 0;

				switch (num)
				{
				case scl::err::SUCCESS:
					return 0;

				case scl::err::FLOAT_ERROR:
					write_number +=
						cfmt::write(file, "error: %s: %s\n", to_string(num),
							fe::to_string());
					break;

				case scl::err::INVALID_ARGUMENT:
				case scl::err::NO_MEMORY:
				case scl::err::NEW:
				case scl::err::INT_OVERFLOW:
				case scl::err::TYPE_CAST:
					 
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
						cfmt::write(file, "error: %s: %s\n", to_string(num), std::strerror(errno));
					break;
				
				default:
					write_number +=
						cfmt::write(file, "error: unknown error!\n");
				}	

				write_number += print_traceback(file);
				return write_number;
			}

			inline void print_exit(FILE *file = nullptr, int exit_code = 1)
			{
				print(file);
				if (num != SUCCESS) 
					exit(exit_code);
			}
		}; // err_t
#pragma pack(pop)


		// default err
		constexpr size_t default_array_size = (1 << 13); // 8129
		info_t default_array[default_array_size];

		constexpr err_t default_err{ num_t::SUCCESS, 0, default_array_size, default_array };
		err_t global_err = default_err;

		// global functions
#define ERR_ARGS __FILE__, __LINE__, __FUNCTION__

#define ERR_CHECK if (scl::err::check_push(__FILE__, __LINE__, __FUNCTION__)) return;
#define ERR_CHECK_VALUE(X) if (scl::err::check_push(__FILE__, __LINE__, __FUNCTION__)) return (X);
#define ERR_CHECK_NO_RETURN scl::err::check_push(__FILE__, __LINE__, __FUNCTION__);

		static inline void set(num_t errnum) { global_err.set(errnum); }
		static inline void clear() { global_err.clear(); }
		static inline void clear_if(num_t num) { global_err.clear_if(num); }
		static inline void set_push(num_t errnum, name_t file, uint64_t line, name_t function) { global_err.set_push(errnum, file, line, function); };
			   
		static inline bool check() { return global_err.check(); }
		static inline bool check_push(name_t file, uint64_t line, name_t function) { return global_err.check_push(file, line, function); }

		static inline void pop() { global_err.pop(); }
		static inline void push(name_t file, uint64_t line, name_t function) { global_err.push(file, line, function); }

		static inline bool test(num_t errnum) { return global_err.test(errnum); }
		static inline const char *string() { return global_err.string(); }

		// print functions	   
		static inline size_t print_traceback(FILE *file = stderr) { return global_err.print_traceback(file); }
		static inline size_t print(FILE *file = stderr) { return global_err.print(file); }
		static inline void print_exit(FILE *file = stderr, int exit_code = 1) { return global_err.print_exit(file, exit_code); }
	}
}

