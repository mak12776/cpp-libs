#pragma once

#include <cstdint>
#include <array>

#include "memory.h"
#include "error.h"

#include "types.h"
#include "io.h"

namespace scl
{
	class obuffer
	{
	private:
		byte *pntr;
		size_t size;

	public:
		inline obuffer(size_t size)
		{
			this->pntr = new byte[size];
			this->size = size;
		}

		inline ~obuffer()
		{
			delete[] this->pntr;
		}

		inline size_t count_lines()
		{
			size_t index;
			size_t total;

			if (this->size == 0)
			{
				throw new_invalid_argument("count_lines", "buffer->size");
				return 0;
			}

			index = 0;
			total = 0;

#define CH (this->pntr[index])
#define END (index == this->size)
#define INC_INDEX_CHECK_END {index += 1; if (END) return total;}

		loop:
			if (CH == '\n')
			{
				total += 1;

				goto next;
			}

		check_cr:
			if (CH == '\r')
			{
				total += 1;

				INC_INDEX_CHECK_END;

				if (CH == '\n')
					goto next;

				goto check_cr;
			}

			index += 1;
			if (END)
			{
				total += 1;
				return total;
			}

			goto loop;

		next:
			INC_INDEX_CHECK_END;

			goto loop;

#undef CH
#undef END
#undef INC_INDEX_CHECK_END
		}
	};

	class view
	{
		size_t start;
		size_t end;
	};

	class packed_view
	{
		byte *pntr;
		view view;
	};

	class buffer_views
	{
		obuffer buffer;
		view *views;
		size_t total;

	public:
		~buffer_views() {};
	};
}

