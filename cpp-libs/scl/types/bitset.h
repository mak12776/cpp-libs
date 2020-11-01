#pragma once

namespace scl
{
	template <size_t _size>
	struct bitset_t
	{
		ubyte pntr[_size];

		inline constexpr size_t size() { return _size; }

		inline void set_all(const bool value)
		{
			memset(pntr, value ? 0xFF : 0, _size);
		}

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
}