#pragma once

#include <stdint.h>
#include "dynamic_array.h"

namespace scl
{
	class buffer : public dynamic_array<uint8_t>
	{
	public:
		buffer(size_t size) : dynamic_array(size)
		{ }

		buffer(size_t size, uint8_t value) : dynamic_array(size, value)
		{ }

		buffer() : dynamic_array()
		{ }

		~buffer()
		{ }

		inline size_type count_lines()
		{
			if (this->empty())
				return 0;


		}
	};
}