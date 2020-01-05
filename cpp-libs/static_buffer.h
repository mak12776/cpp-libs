#pragma once

#include <cstdlib>
#include "static_array.h"

namespace scl
{
	template <std::size_t _size>
	class static_buffer : public static_array<uint8_t, _size>
	{
	public:
		typedef uint8_t* pointer;

		static_buffer(pointer data)
			: static_array<uint8_t, _size>(data)
		{ }
	};
}