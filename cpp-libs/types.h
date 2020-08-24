#pragma once

#include <cstdint>
#include <climits>

namespace scl
{
	typedef int8_t byte;
	typedef uint8_t ubyte;

#define BYTE_MAX	INT8_MAX
#define BYTE_MIN	INT8_MIN
#define UBYTE_MAX	UINT8_MAX

#if SIZE_MAX == UINT8_MAX
	typedef int8_t ssize_t;
	constexpr uint8_t size_bytes = 1;
#elif SIZE_MAX == UINT16_MAX
	typedef int16_t ssize_t;
	constexpr uint8_t size_bytes = 2;
#elif SIZE_MAX == UINT32_MAX
	typedef int32_t ssize_t;
	constexpr uint8_t size_bytes = 4;
#elif SIZE_MAX == UINT64_MAX
	typedef int64_t ssize_t;
	constexpr uint8_t size_bytes = 8;
#else
#error unknown SIZE_MAX
#endif

}
