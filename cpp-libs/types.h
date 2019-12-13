#pragma once

#include <cstdint>

namespace scl
{
	typedef unsigned char uchar;

	typedef char* string_t;
	typedef uchar* ustring_t;

	typedef uint8_t u8char_t;
	typedef int8_t s8char_t;

	typedef u8char_t* u8string_t;
	typedef s8char_t* s8string_t;

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
