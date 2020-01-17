#pragma once

#include <array>

namespace scl
{
	template <size_t _size>
	struct buffer : public std::array<char, _size>
	{

	};
}