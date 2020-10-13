#pragma once

#include <cstdlib>
#include "array_pointer.h"

namespace scl
{
	template <std::size_t _size>
	class buffer_pointer : public array_pointer<char, _size>
	{
	public:
		typedef char* pointer;
		typedef const char* const_pointer;

		typedef char& reference;
		typedef const char& const_reference;

		buffer_pointer(const_pointer data)
			: array_pointer<char, _size>(data)
		{ }

		buffer_pointer(const_pointer data, const_reference value)
			: array_pointer<char, _size>(data, value)
		{ }
	};
}