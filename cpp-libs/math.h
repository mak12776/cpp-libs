#pragma once

#include <cstdint>
#include <stdexcept>
#include <safeint.h>

namespace scl
{
	namespace math
	{
		// generic functions

		template<typename type>
		static inline bool mul(type a, type b, type &result)
		{
#ifdef _MSC_VER
			return !msl::utilities::SafeMultiply(a, b, result);
#elif defined(__GNUC__)
			return __builtin_mul_overflow(a, b, &result);
#else
#error unknown compiler
#endif
		}

		template<typename type>
		static inline bool add(type a, type b, type &result)
		{
#ifdef _MSC_VER
			return !msl::utilities::SafeAdd(a, b, result);
#elif defined(__GNUC__)
			return __builtin_add_overflow(a, b, &result);
#else
#error unknown compiler
#endif
		}


		// safe functions

		template<typename type>
		static inline void safe_mul(type a, type b, type &result)
		{
			if (mul<type>(a, b, result))
				throw new std::overflow_error("multiplay overflow");
		}

		template<typename type>
		static inline void safe_add(type a, type b, type &result)
		{
			if (add<type>(a, b, result))
				throw new std::overflow_error("addition overflow");
		}


		// specific functions

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
#else 
#error incomplete function
#endif // SIZE_MAX == UINT32_MAX

#elif defined(__GNUC__)
			return __builtin_mul_overflow(a, b, &result);
#else
#error unknown compiler
#endif // _MSC_VER
		}

		static inline void safe_mul_size(size_t a, size_t b, size_t &result)
		{
			if (mul_size(a, b, result))
				throw new std::overflow_error("size multiply overflow");
		}
	}
}