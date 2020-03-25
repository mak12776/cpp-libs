#pragma once

#include <cstdlib>
#include "array_pointer.h"

namespace scl
{
	template <std::size_t _size>
	class buffer_pointer : public array_pointer<char, _size>
	{
	public:

		typedef uint8_t* reference;
		typedef const uint8_t* const_reference;

		typedef uint8_t* pointer;
		typedef const uint8_t* const_pointer;

		buffer_pointer(const_pointer data)
			: array_pointer<uint8_t, _size>(data)
		{ }

		buffer_pointer(const_pointer data, const_reference value)
			: array_pointer<uint8_t, _size>(data, value)
		{ }
	};
}