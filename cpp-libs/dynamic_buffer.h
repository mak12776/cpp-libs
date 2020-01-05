#pragma once

#include <stdint.h>
#include "dynamic_array.h"

namespace scl
{
	class dynamic_buffer : public dynamic_array<char>
	{
	public:
		dynamic_buffer() : dynamic_array()
		{ }

		dynamic_buffer(size_type size) : dynamic_array(size)
		{ }

		dynamic_buffer(size_type size, const_reference value) : dynamic_array(size, value)
		{ }

		// others

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