#pragma once

#include <array>

namespace scl
{
	template <size_t _size>
	struct buffer : public std::array<uint8_t, _size>
	{

	};
}