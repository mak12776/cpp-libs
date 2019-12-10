#pragma once

#include <cctype>

#include "types.h"

namespace scl
{
	namespace string
	{
		static inline size_t find_char(char_t *pntr, size_t start, size_t end, char_t ch)
		{
			size_t index;

			index = start;
			while (index < end)
			{
				if (pntr[index] == ch)
					return index;
				index += 1;
			}
			return end;
		}

		static inline size_t rfind_char(char_t *pntr, size_t start, size_t end, char_t ch)
		{
			size_t index;

			index = end;
			while (index > start)
			{
				index -= 1;
				if (pntr[index] == ch)
					return index;
			}
			return end;
		}

		static inline size_t find_test(char_t *pntr, size_t start, size_t end, int (*test) (int))
		{
			size_t index;

			index = start;
			while (index < end)
			{
				if (test(pntr[index]))
					return index;
				index += 1;
			}
			return end;
		}

		static inline size_t rfind_test(char_t *pntr, size_t start, size_t end, int (*test) (int))
		{
			size_t index;

			index = end;
			while (index > start)
			{
				index -= 1;
				if (test(pntr[index]))
					return index;
			}
			return end;
		}
	}
}