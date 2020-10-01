#pragma once

#include <cstdint>
#include <climits>
#include <cinttypes>

namespace scl
{
	typedef int8_t byte;
	typedef uint8_t ubyte;

	struct ubuffer_t
	{
		ubyte *pntr;
		size_t size;
	};

	struct buffer_t
	{
		byte *pntr;
		size_t size;
	};

	template <size_t _size>
	struct bitset_t
	{
		ubyte pntr[_size];

		inline constexpr size_t size() { return _size; }

		inline void set_all(const bool value)
		{ memset(pntr, value ? 0xFF : 0, _size); }

		inline bool get(size_t index)
		{
			constexpr size_t uintmax_size = sizeof(uintmax_t);
			size_t reminder = uintmax_size - 1 - (index % uintmax_size);

			return *((uintmax_t *)pntr + (index / uintmax_size))
				& ((uintmax_t)1 << reminder);
		}

		inline void set(size_t index, bool value)
		{
			constexpr size_t uintmax_size = sizeof(uintmax_t);
			size_t reminder = uintmax_size - 1 - (index % uintmax_size);

			*((uintmax_t *)pntr + (index / uintmax_size))
				|= ((uintmax_t)1 << reminder);
		}
	};

#define BYTE_MAX	INT8_MAX
#define BYTE_MIN	INT8_MIN
#define UBYTE_MAX	UINT8_MAX

#if SIZE_MAX == UINT8_MAX
	typedef int8_t ssize_t;

	constexpr uint8_t size_bytes = 1;
	constexpr ubyte size_bits = 8;

#elif SIZE_MAX == UINT16_MAX
	typedef int16_t ssize_t;

	constexpr uint8_t size_bytes = 2;
	constexpr ubyte size_bits = 16;

#elif SIZE_MAX == UINT32_MAX
	typedef int32_t ssize_t;

	constexpr uint8_t size_bytes = 4;
	constexpr ubyte size_bits = 32;

#elif SIZE_MAX == UINT64_MAX
	typedef int64_t ssize_t;

	constexpr ubyte size_bytes = 8;
	constexpr ubyte size_bits = 64;

#else
#error unknown SIZE_MAX
#endif
}
