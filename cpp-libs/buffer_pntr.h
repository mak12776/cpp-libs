#pragma once

#include <cstdlib>
#include "array_pntr.h"

namespace scl
{
	template <std::size_t _size>
	class buffer_pntr : public array_pntr<char, _size>
	{
	public:
		typedef uint8_t* reference;
		typedef const uint8_t* const_reference;

		typedef uint8_t* pointer;
		typedef const uint8_t* const_pointer;

		buffer_pntr(const_pointer data)
			: array_pntr<uint8_t, _size>(data)
		{ }

		buffer_pntr(const_pointer data, const_reference value)
			: array_pntr<uint8_t, _size>(data, value)
		{ }
	};
}