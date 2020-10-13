#pragma once

#include <cctype>

#include "types.h"

namespace scl
{
#if 0
	namespace string
	{
		static inline size_t find_char(byte *pntr, size_t start, size_t end, byte ch)
		{
			size_t len;

			len = start;
			while (len < end)
			{
				if (pntr[len] == ch)
					return len;
				len += 1;
			}
			return end;
		}

		static inline size_t rfind_char(byte *pntr, size_t start, size_t end, byte ch)
		{
			size_t len;

			len = end;
			while (len > start)
			{
				len -= 1;
				if (pntr[len] == ch)
					return len;
			}
			return end;
		}

		static inline size_t find_test(byte *pntr, size_t start, size_t end, int (*test) (int))
		{
			size_t len;

			len = start;
			while (len < end)
			{
				if (test(pntr[len]))
					return len;
				len += 1;
			}
			return end;
		}

		static inline size_t rfind_test(byte *pntr, size_t start, size_t end, int (*test) (int))
		{
			size_t len;

			len = end;
			while (len > start)
			{
				len -= 1;
				if (test(pntr[len]))
					return len;
			}
			return end;
		}
	}
#endif
}
