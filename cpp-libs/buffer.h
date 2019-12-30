#pragma once

#include <stdint.h>
#include "dynamic_array.h"

namespace scl
{
	class buffer : public dynamic_array<uint8_t>
	{
	public:
		buffer() : dynamic_array()
		{ }

		buffer(size_t size) : dynamic_array(size)
		{ }

		buffer(size_t size, uint8_t value) : dynamic_array(size, value)
		{ }

		~buffer()
		{ }

		inline size_type count_lines()
		{
			size_type index;
			size_type total;

			if (this->empty())
				return 0;

			index = 0;
			total = 0;

#define CH (this->_data[index])
#define END (index == this->_capacity)
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

	// more types: view, packed view, buffer views
}