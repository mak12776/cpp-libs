#pragma once

#include "static_array.h"

namespace scl
{
	template <std::size_t _size>
	class static_buffer: public static_array<uint8_t, _size>
	{
	public:
		static_buffer()
		{

		}
	};
}