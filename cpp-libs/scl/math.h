#pragma once

#include <cmath>
#include <cfenv>

namespace scl
{
	namespace math
	{
		template <typename src_type, typename dest_type>
		bool cast_value(src_type src, dest_type &dest)
		{
			constexpr auto src_digits = std::numeric_limits<src_type>::digits;
			constexpr auto dest_digits = std::numeric_limits<dest_type>::digits;

			constexpr auto src_is_integral = std::is_integral_v<src_type>;
			constexpr auto dest_is_integral = std::is_integral_v<dest_type>;

			constexpr auto src_is_float = std::is_floating_point_v<src_type>;
			constexpr auto dest_is_float = std::is_floating_point_v<dest_type>;

			if constexpr (std::is_same_v<src_type, dest_type>)
			{
				dest = src;
				return false;
			}

			if (src_type(dest_type(src)) != src)
				return true;

			dest = (dest_type)src;
			return false;
		}

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

		

		template <typename type, typename float_type = long double>
		static inline bool pow(type base, type exp, type &result)
		{
			type temp;

			float_type base_float, exp_float, result_float;

			if constexpr (std::is_floating_point_v<type>)
				temp = std::pow<type, type>(base, exp);
			else if constexpr (std::is_integral_v<type>)
			{
				if (cast_value(base, base_float) || cast_value(exp, exp_float))
					return true;

				result_float = std::pow<float_type, float_type>(base_float, exp_float);

				if (cast_value(result_float, temp))
					return true;
			}

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

			result = temp;
			return false;
		}

		static inline const char *fe_string()
		{
			switch(std::fetestexcept(FE_ALL_EXCEPT))
			{
#ifdef FE_DIVBYZERO
			case FE_DIVBYZERO: return "FE_DIVBYZERO";
#endif
#ifdef FE_INEXACT
			case FE_INEXACT: return "FE_INEXACT";
#endif
#ifdef FE_INVALID
			case FE_INVALID: return "FE_INVALID";
#endif
#ifdef FE_OVERFLOW
			case FE_OVERFLOW: return "FE_OVERFLOW";
#endif
#ifdef FE_UNDERFLOW
			case FE_UNDERFLOW: return "FE_UNDERFLOW";
#endif
			default: return "UNKNOWN";
			}
		}

		// safe functions

		template <typename int_type, typename float_type>
		void safe_cast_value(int_type value, float_type &result)
		{
			if (cast_value(value, result))
			{
				err::set(err::INT_OVERFLOW);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

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

		template <typename type, typename float_type = long double>
		static inline void safe_pow(type base, type exp, type &res)
		{
			if (pow<type, float_type>(base, exp, res))
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