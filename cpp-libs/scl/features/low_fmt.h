#pragma once

namespace scl
{
	namespace low_fmt
	{
		// --- low format ---
		// using stdio printf family functions with better function names.
		// doesn't support sized strings.

		static inline size_t cast_int(int value)
		{
#if INT_MAX > SIZE_MAX
#error INT_MAX is to big.
#endif		
			return (size_t)value;
		}

		static inline bool get_len_valist(size_t &len, const char *fmt, va_list list)
		{
			int ret;

			ret = _vscprintf_p(fmt, list);
			if (ret < 0)
				return true;

			len = cast_int(ret);
			return false;
		}

		static inline size_t safe_get_len_valist(const char *fmt, va_list list)
		{
			size_t len = 0;
			if (get_len_valist(len, fmt, list))
			{
				err::set(err::PRINTF);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
			return len;
		}

		static inline bool format_valist(size_t &len, char *pntr, size_t size, const char *fmt, va_list list)
		{
			int ret;

			ret = _vsprintf_p(pntr, size, fmt, list);
			if (ret < 0)
				return true;

#if INT_MAX > SIZE_MAX
#error INT_MAX is to big.
#endif
			len = cast_int(ret);
			return false;
		}

		static inline size_t safe_format_valist(char *pntr, size_t size, const char *fmt, va_list list)
		{
			size_t len = 0;
			if (format_valist(len, pntr, size, fmt, list))
			{
				err::set(err::PRINTF);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
			return len;
		}

		static inline bool malloc_format_valist(char *&pntr, size_t &size, const char *fmt, va_list list)
		{
			size_t len;
			va_list list_copy;

			if (get_len_valist(len, fmt, list))
				return true;

		}
	}
}
