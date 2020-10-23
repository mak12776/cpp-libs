#pragma once

namespace scl
{
	namespace fmt
	{
		/* {0:i32} {1:i32}
		*/
		enum class num_t : uint8_t
		{
			SUCCESS = 0,
			UNMATCHED_CURLY_BRACKET
		};

		static inline size_t get_len(const char *fmt, size_t len)
		{
			size_t index = 0;
			size_t total = len;

			while (index < len)
			{
				if (fmt[index] == '{')
				{
					index += 1;
					if (index == len)
						return total;

					if (fmt[index] == '{')
						total -= 1;
				}
				index += 1;
			}
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