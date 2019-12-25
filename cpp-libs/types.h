#pragma once

#include <cstdint>
#include <climits>

namespace scl
{
	typedef unsigned char uchar;

	typedef char* string_t;
	typedef uchar* ustring_t;

	typedef uint8_t uchar8_t;
	typedef int8_t char8_t;

	typedef uchar8_t* ustring8_t;
	typedef char8_t* string8_t;

	typedef uint8_t byte;				/* used for IO structures */

#if SIZE_MAX == UINT8_MAX
	typedef int8_t ssize_t;
#elif SIZE_MAX == UINT16_MAX
	typedef int16_t ssize_t;
#elif SIZE_MAX == UINT32_MAX
	typedef int32_t ssize_t;
#elif SIZE_MAX == UINT64_MAX
	typedef int64_t ssize_t;
#else
#error unknown SIZE_MAX
#endif
}
