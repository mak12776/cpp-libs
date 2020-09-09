
// c printf functions

#include "pch.h"

#include "math.h"
#include "clib.h"

namespace cl
{

#ifdef _MSC_VER
	int vasprintf(const char **strp, const char *fmt, va_list ap)
	{
		using namespace scl;

		va_list ap_copy;
		char *str;
		size_t size;
		int ret;

		// _vsprintf will not return negative values, or failed.
		va_copy(ap_copy, ap);
		ret = _vscprintf(fmt, ap_copy);

		if (math::add((size_t)ret, (size_t)1, size))
			return INT_ERROR;

		str = (char *)malloc(size);
		if (!str)
			return INT_ERROR;

		ret = vsprintf(str, fmt, ap);
		if (ret < 0)
		{
			free(str);
			return INT_ERROR;
		}

		*strp = str;
		return ret;
	}

	int asprintf(const char **strp, const char *fmt, ...)
	{
		using namespace scl;

		va_list ap;
		int ret;

		va_start(ap, fmt);
		ret = vasprintf(strp, fmt, ap);
		va_end(ap);

		return ret;
	}
#endif

	int printf_ln(const char *fmt, ...)
	{
		const char *str;
		int ret;
		va_list ap;

		va_start(ap, fmt);
		ret = vasprintf(&str, fmt, ap);
		va_end(ap);

		if (ret < 0)
			return INT_ERROR;

		ret = printf("%s\n", str);
		free((void *)str);

		return ret;
	}

	template <typename value_type>
	int print_size_of(FILE *stream)
	{
		return fprintf(stream, "sizeof %s: %zu\n", typeid(value_type).name(),
			sizeof(value_type));
	}

	template <typename value_type>
	int print_address_of(value_type *value, const char *name, FILE *stream)
	{
		if (name == nullptr) name = "...";
		return fprintf(stream, "address of %s: %zu\n", name, value);
	}
}
