#pragma once

namespace scl
{
	typedef int8_t byte;
	typedef uint8_t ubyte;

	template <typename byte_type>
	struct base_buffer_t
	{
		byte_type *pntr;
		size_t size;
	};

	typedef base_buffer_t<void> vbuffer_t;
	typedef base_buffer_t<ubyte> ubuffer_t;
	typedef base_buffer_t<byte> buffer_t;

	template <size_t _size>
	struct bitset_t
	{
		ubyte pntr[_size];

		inline constexpr size_t size() { return _size; }

		inline void set_all(const bool value)
		{ memset(pntr, value ? 0xFF : 0, _size); }

		inline bool get(size_t len)
		{
			constexpr size_t uintmax_size = sizeof(uintmax_t);
			size_t remaining = uintmax_size - 1 - (len % uintmax_size);

			return *((uintmax_t *)pntr + (len / uintmax_size))
				& ((uintmax_t)1 << remaining);
		}

		inline void set(size_t len, bool value)
		{
			constexpr size_t uintmax_size = sizeof(uintmax_t);
			size_t remaining = uintmax_size - 1 - (len % uintmax_size);

			*((uintmax_t *)pntr + (len / uintmax_size))
				|= ((uintmax_t)1 << remaining);
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