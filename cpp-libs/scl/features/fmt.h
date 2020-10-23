#pragma once

namespace scl
{
	namespace fmt
	{
		// c standards: 127 parameters in one function definition
		constexpr uint8_t max_argument_index = 127 - 1;

		enum class fmt_num_t : uint8_t
		{
			SUCCESS = 0, 
			UNMATCHED_CURLY_BRACKET,
			SINGLE_LEFT_CURLY_BRACKET,
			SINGLE_RIGHT_CURLY_BRACKET,
		};

		struct fmt_t
		{
			fmt_num_t num;
			size_t pos;

			inline void set(fmt_num_t errnum, size_t position)
			{ num = errnum; pos = position; }
			inline void clear() { num = fmt_num_t::SUCCESS; }
		};

		fmt_t default_fmt{ fmt_num_t::SUCCESS, 0 };
		fmt_t global_fmt = default_fmt;

		static inline void set(fmt_num_t errnum, size_t index) 
		{ global_fmt.set(errnum, index); }

		static inline void clear() { global_fmt.clear(); }

		// writing these library with different formats takes a lot of time,
		// use a scripting programming language to produce the needed grammer 
		// parser.

		// deal with null-terminated strings or usual strings? support both types.

		// library functions:

		// write to a file

		// get length of fmt
		// write to a buffer
		// allocate buffer and write into

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
				set(fmt_num_t::SINGLE_RIGHT_CURLY_BRACKET, index - 1);
				return invalid_total;
			}

			index += 1;
			total -= 1;
			goto main_check;

		left_curly_bracket:

			if (index == len)
			{
				set(fmt_num_t::SINGLE_LEFT_CURLY_BRACKET, index - 1);
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
				
			}

		colon:

		}

		static inline size_t malloc_fmt(const char *fmt, ...)
		{

		}
	}

	template <typename value_type>
	static inline int print_size_of(FILE *stream = stdout)
	{
		return fprintf(stream, "sizeof %s: %zu\n", typeid(value_type).name(),
			sizeof(value_type));
	}

	template <typename value_type>
	static inline int print_address_of(value_type *value, const char *name = nullptr, FILE *stream = stdout)
	{
		if (name == nullptr) name = "...";
		return fprintf(stream, "address of %s: %zu\n", name, value);
	}

	static inline size_t safe_fprintf(FILE *stream, const char *fmt, ...)
	{
		va_list ap;
		int ret;

		va_start(ap, fmt);
		ret = vfprintf(stream, fmt, ap);
		va_end(ap);

		if (ret < 0)
		{
			scl::err::set(scl::err::PRINTF);
			scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

			return 0;
		}

		return (size_t)ret;
	}

	static inline size_t safe_printf(const char *fmt, ...)
	{

		va_list ap;
		int ret;

		va_start(ap, fmt);
		ret = vprintf(fmt, ap);
		va_end(ap);

		if (ret < 0)
		{
			scl::err::set(scl::err::PRINTF);
			scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

			return 0;
		}

		return (size_t)ret;
	}

}