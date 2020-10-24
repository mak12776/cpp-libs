#pragma once

namespace scl
{
	namespace fmt
	{
		// c standards: 127 parameters in one function definition
		constexpr uint8_t max_argument_index = 127 - 1;

		enum class num_t : uint8_t
		{
			SUCCESS = 0, 
			UNMATCHED_CURLY_BRACKET,
			SINGLE_LEFT_CURLY_BRACKET,
			SINGLE_RIGHT_CURLY_BRACKET,
		};

		struct fmt_t
		{
			num_t num;
			size_t pos;

			inline void set(num_t errnum, size_t position) { num = errnum; pos = position; }
			inline bool check() { return num != num_t::SUCCESS; }
			inline void clear() { num = num_t::SUCCESS; }
		};

		fmt_t default_fmt{ num_t::SUCCESS, 0 };
		fmt_t global_fmt = default_fmt;

		static inline void set(num_t errnum, size_t index) { global_fmt.set(errnum, index); }
		static inline void clear() { global_fmt.clear(); }
		static inline bool check() { return global_fmt.check(); }

		namespace low
		{
			/* 
				=== grammar ===

				replacement: "{" [[argument_location] ":"] format "}"

				argument_location: digit+
				digit: "0" .. "9"
				format: int_format | float_format

				int_format: int_specifier [int_length | int_width]
				int_length: "hh" | "h" | "l" | "ll"
				int_width: digit+
				int_specifier: "i" | "u" | "o" | "x" | "X"

				float_specifier: ["L"] ("f" | "F" | "e" | "E" | "g" | "G")

				char_format: "c"
				null_string_format: "s"
				pointer_string_format: "p"

				--- format specifier table ---

				int types:

				"i"						"u" "o" "x" "X"				size specifier

				int?_t					uint?_t						"8" "16" "32" "64"

				char					unsigned char				"hh"
				short int				unsigned short int			"h"
				int						unsigned int				""
				long int				unsigned long int			"l"
				long long int			unsigned long long int		"ll"

				intmax_t				uintmax_t					"!"
				size_t					size_t						"#"


				float types:

				"f" "F" "e" "E" "g" "G"

			*/

			static inline size_t get_len_valist(const char *fmt, size_t len, fmt_t &err, va_list valist)
			{
				// the following "//#{ ... }" marks needed for a python script to locate 
				// the body of 'get_len' & 'format' functions and place generated algorithms 
				// inside them. please don't remove.

				//#[get_line_valist start]
				//#[get_line_valist end]

				size_t index = 0;
				size_t total = len;

			main_check:
				if (index == len)
					return total;

				switch (fmt[index])
				{
				case '{':
					index += 1;
					goto left_curly_bracket;

				case '}':
					index += 1;
					goto right_curly_bracket;

				default:
					index += 1;
					goto main_check;
				}

			right_curly_bracket:
				if ((index == len) || (fmt[index] != '}'))
				{
					set(num_t::SINGLE_RIGHT_CURLY_BRACKET, index - 1);
					constexpr size_t invalid_total = 0;
				}

				index -= 1;
				total -= 1;
				goto main_check;

			left_curly_bracket:

			}

			static inline void format_valist(char **string, const char *fmt, size_t len, fmt_t &err, va_list valist)
			{
				//#[format_valist start]
				//#[format_valist end]
			}
		}

		static inline size_t get_len(const char *fmt, size_t len, ...)
		{

		}

		static inline void format(char **string, const char *fmt, size_t len, ...)
		{

		}

		// == fmt library ==

		// -- decription --
		// writing these library with different formats takes a lot of time,
		// use a scripting programming language to produce the needed grammer 
		// parser.

		// -- features --
		// deal with null-terminated strings or sized strings? support both types.

		// library functions:

		// get length of fmt
		// write to a buffer
		// allocate buffer and write into

		// write to a file

		static inline size_t get_len(const char *fmt, size_t len)
		{
			size_t index = 0;
			size_t total = len;
			constexpr size_t invalid_total = 0;

			// "finite state machine"

		main_check:
			if (index == len)
				return total;

		main_switch:
			switch (fmt[index])
			{
			case '{': 
				index += 1;
				goto left_curly_bracket;

			case '}':
				index += 1;
				goto right_curly_bracket;

			default:
				index += 1;
				goto main_check;
			}

		right_curly_bracket:
			if ((index == len) || (fmt[index] != '}'))
			{
				set(num_t::SINGLE_RIGHT_CURLY_BRACKET, index - 1);
				return invalid_total;
			}

			index += 1;
			total -= 1;
			goto main_check;

		left_curly_bracket:

			if (index == len)
			{
				set(num_t::SINGLE_LEFT_CURLY_BRACKET, index - 1);
				return invalid_total;
			}

			switch (fmt[index])
			{
			case '{':				// double_left_curly_bracket
				index += 1;
				total -= 1;
				goto main_check;

			case ':':
				index += 1;
				goto colon;

			default:
				break;
			}

		colon:
			return 0;
		}
	}
}