#pragma once

#include <cmath>
#include <cfenv>

namespace scl
{
	namespace math
	{
		// simple primes finder

		// safe arithmetic operations

		template<typename type>
		static inline bool mul(type a, type b, type &res)
		{
#ifdef _MSC_VER
			return !msl::utilities::SafeMultiply(a, b, res);
#elif defined(__GNUC__)
			return __builtin_mul_overflow(a, b, &res);
#else
#error unknown compiler
#endif
		}

		template <typename type>
		static inline bool sub(type a, type b, type &res)
		{
#ifdef _MSC_VER
			return !msl::utilities::SafeMultiply(a, b, res);
#elif defined(__GNUC__)
			return __builtin_sub_overflow(a, b, &res);
#else
#error unknown compiler
#endif
		}

		template<typename type>
		static inline bool add(type a, type b, type &res)
		{
#ifdef _MSC_VER
			return !msl::utilities::SafeAdd(a, b, res);
#elif defined(__GNUC__)
			return __builtin_add_overflow(a, b, &res);
#else
#error unknown compiler
#endif
		}

		template <typename type>
		static inline bool pow(type base, type exp, type &res)
		{
			if (std::numeric_limits<type>::digits10 >= std::numeric_limits<long double>::digits10)
				return true;

			res = std::pow<long double, long double>(base, exp);
			if (math_errhandling & MATH_ERREXCEPT)
			{
				if (std::fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW))
					return true;
			}
			else if constexpr (math_errhandling & MATH_ERRNO)
			{
				if ((errno == EDOM) or (errno == ERANGE))
					return true;
			}

			return false;
		}

		// safe functions

		template<typename type>
		static inline void safe_mul(type a, type b, type &result)
		{
			if (mul<type>(a, b, result))
			{
				err::set(err::INT_OVERFLOW);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		template<typename type>
		static inline void safe_add(type a, type b, type &result)
		{
			if (add<type>(a, b, result))
			{
				err::set(err::INT_OVERFLOW);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		template <typename type>
		static inline void safe_sub(type a, type b, type &result)
		{
			if (sub<type>(a, b, result))
			{
				err::set(err::INT_OVERFLOW);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		template <typename type>
		static inline void safe_pow(type base, type exp, type &res)
		{
			if (pow<type>(base, exp, res))
			{
				err::set(err::FLOAT_OVERFLOW);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		// specific functions

#if SCL_DEPRECATED

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

#endif

	}
}