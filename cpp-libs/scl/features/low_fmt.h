#pragma once

namespace scl
{
	namespace low_fmt
	{
		// --- low format ---
		// using stdio printf family functions with better function names.
		// doesn't support sized strings.

		static inline size_t get_len_valist(const char *fmt, va_list list)
		{
			int ret;

			ret = _vscprintf(fmt, list);
		}
	}
}
