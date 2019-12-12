#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <safeint.h>

namespace scl
{
	namespace math
	{
		static inline bool mul_size(size_t a, size_t b, size_t &result)
		{
#ifdef _MSC_VER

#if SIZE_MAX == UINT32_MAX
			typedef uint64_t size_overflow_t;
			const uint32_t SIZE_BITS = 32;

			size_overflow_t prod = (size_overflow_t)a * (size_overflow_t)b;
			result = (size_t)prod;
			return (prod >> SIZE_BITS) != 0;
#elif SIZE_MAX == UINT64_MAX
			return !msl::utilities::SafeMultiply(a, b, result);
#endif // SIZE_MAX == UINT64_MAX

#elif defined(__GNUC__)

#error incomplete code

#endif // defined(__GNU_C__)
		}

		static inline void safe_mul_size(size_t a, size_t b, size_t &result)
		{
			if (mul_size(a, b, result))
				error::num = error::ERROR_INT_OVERFLOW;
		}
	}
}