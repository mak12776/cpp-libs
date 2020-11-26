#pragma once

#include <cmath>
#include <cfenv>
#include <iostream>

namespace scl
{
	namespace fe
	{
		// float environment

		static inline const char *to_string()
		{
			switch (std::fetestexcept(FE_ALL_EXCEPT))
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

		static inline bool check_except()
		{
			if constexpr ((math_errhandling & MATH_ERREXCEPT) != 0)
				return (std::fetestexcept(FE_ALL_EXCEPT) != 0);
			else if constexpr ((math_errhandling & MATH_ERRNO) != 0)
				return (errno == EDOM) || (errno == ERANGE);
			return true;
		}
	}

	namespace math
	{
		template <typename src_type, typename dest_type>
		bool cast_value(src_type src, dest_type &dest)
		{
			constexpr auto src_digits = std::numeric_limits<src_type>::digits;
			constexpr auto dest_digits = std::numeric_limits<dest_type>::digits;

			constexpr auto src_max_exp = std::numeric_limits<src_type>::max_exponent;
			constexpr auto dest_max_exp = std::numeric_limits<dest_type>::max_exponent;

			constexpr auto src_is_integral = std::is_integral_v<src_type>;
			constexpr auto dest_is_integral = std::is_integral_v<dest_type>;

			constexpr auto src_is_float = std::is_floating_point_v<src_type>;
			constexpr auto dest_is_float = std::is_floating_point_v<dest_type>;

			constexpr auto check_digits_exp =
				(src_digits > dest_digits) || (src_max_exp > dest_max_exp);

			if constexpr (std::is_same_v<src_type, dest_type>)
			{
				dest = src;
				return false;
			}

			if constexpr (
				(src_is_integral
					&& (dest_is_integral || dest_is_float)
					&& (src_digits > dest_digits))
				|| (src_is_float && dest_is_float && check_digits_exp)
				|| (src_is_float && dest_is_integral))
			{
				if (src_type(dest_type(src)) != src)
					return true;
			}

			dest = (dest_type)src;
			return false;
		}

		// safe arithmetic operations

		template <typename type>
		static inline bool upper_bounad(type value, type divisor, type &result)
		{
			type remaining = (value % divisor);
			if (remaining == 0)
			{
				result = value;
				return false;
			}
			
		}

		template <typename type>
		static inline void lower_bound(type value, type divisor, type &res)
		{
			return value - (value % divisor);
		}

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

		template <typename type, typename suggested_type = long double>
		static inline bool pow(type base, type exp, type &result)
		{
			using float_type = std::conditional_t<
				std::is_floating_point_v<suggested_type>,
				suggested_type, long double>;

			type temp_result;
			float_type base_float, exp_float, result_float;

			if constexpr (std::is_floating_point_v<type>)
				temp_result = std::pow<type, type>(base, exp);
			else if constexpr (std::is_integral_v<type>)
			{
				if (cast_value(base, base_float) || cast_value(exp, exp_float))
					return true;

				result_float = std::pow<float_type, float_type>(base_float, exp_float);

				if (cast_value(result_float, temp_result))
					return true;
			}

			if (fe::check_except())
				return true;

			result = temp_result;
			return false;
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

		static inline bool check_pow_error()
		{
			if constexpr ((math_errhandling & MATH_ERREXCEPT) == 1)
			{
				if (std::fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW))
					return true;
			}
			else if constexpr ((math_errhandling & MATH_ERRNO) == 1)
			{
				if ((errno == EDOM) || (errno == ERANGE))
					return true;
			}
			return false;
		}

		template <typename type, typename suggested_type = long double>
		static inline void safe_pow(type base, type exp, type &result)
		{
			using float_type = std::conditional_t<
				std::is_floating_point_v<suggested_type>, 
				suggested_type, long double>;

			type temp_result;
			float_type base_float, exp_float, result_float;

			if constexpr (std::is_floating_point_v<type>)
			{
				temp_result = std::pow<type, type>(base, exp);
				if (check_pow_error())
				{
					err::set(err::FLOAT_ERROR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
					return;
				}
			}
			else if constexpr (std::is_integral_v<type>)
			{
				safe_cast_value(base, base_float);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;

				safe_cast_value(exp, exp_float);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;

				result_float = std::pow<type, type>(base, exp);
				if (check_pow_error())
				{
					err::set(err::FLOAT_ERROR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
					return;
				}

				safe_cast_value(result_float, temp_result);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}

			result = temp_result;
		}
	}
}