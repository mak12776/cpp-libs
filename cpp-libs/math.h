#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <safeint.h>

namespace scl
{
	namespace math
	{
		template<typename T>
		static inline bool mul(T a, T b, T &result)
		{
#ifdef _MSC_VER
			return !msl::utilities::SafeMultiply(a, b, result);
#elif defined(__GNUC__)
			return __builtin_mul_overflow(a, b, &result);
#endif // _MSC_VER
		}

		template<typename T>
		static inline bool safe_mul(T a, T b, T &result)
		{
			if (mul<T>(a, b, result))
				error::set_error_int_overflow();
		}

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
#endif // SIZE_MAX == UINT32_MAX
#elif defined(__GNUC__)
			return __builtin_mul_overflow(a, b, &result);
#endif // _MSC_VER
		}

		static inline void safe_mul_size(size_t a, size_t b, size_t &result)
		{
			if (mul_size(a, b, result))
				error::set_error_int_overflow();
		}
	}
}