#pragma once

namespace scl
{
	namespace winapi
	{
		// FileTime, ULARGE_INTEGER conversation

		static inline void file_time_to_ularge_integer(FILETIME &ft, ULARGE_INTEGER &ul_int)
		{
			ul_int.HighPart = ft.dwHighDateTime;
			ul_int.LowPart = ft.dwLowDateTime;
		}

		static inline void ularge_integer_to_file_time(ULARGE_INTEGER &ul_int, FILETIME &ft)
		{
			ft.dwHighDateTime = ul_int.HighPart;
			ft.dwLowDateTime = ul_int.LowPart;
		}

		// math operations between ularge integer & file time

		static inline void sub_ularge_integer(ULARGE_INTEGER &ul_int1, ULARGE_INTEGER &ul_int2, ULARGE_INTEGER &res)
		{
			res.QuadPart = ul_int1.QuadPart - ul_int2.QuadPart;
		}

		static inline void sub_ularge_integer(ULARGE_INTEGER &ul_int1, ULARGE_INTEGER &ul_int2)
		{
			ul_int1.QuadPart = ul_int1.QuadPart - ul_int2.QuadPart;
		}

		static inline void sub_file_time(FILETIME &ft1, FILETIME &ft2, FILETIME &res)
		{
			ULARGE_INTEGER ul_int1, ul_int2, ul_int_res;

			file_time_to_ularge_integer(ft1, ul_int1);
			file_time_to_ularge_integer(ft2, ul_int2);

			sub_ularge_integer(ul_int1, ul_int2, ul_int_res);

			ularge_integer_to_file_time(ul_int_res, res);
		}

		inline void sub_file_time(FILETIME &ft1, FILETIME &ft2)
		{
			ULARGE_INTEGER ul_int1, ul_int2;

			file_time_to_ularge_integer(ft1, ul_int1);
			file_time_to_ularge_integer(ft2, ul_int2);

			sub_ularge_integer(ul_int1, ul_int2);

			ularge_integer_to_file_time(ul_int2, ft2);
		}

		// safe FileTime, SystemTime conversations

		static inline void safe_file_time_to_system_time(FILETIME &ft, SYSTEMTIME &st)
		{
			if (!FileTimeToSystemTime(&ft, &st))
			{
				scl::err::set(scl::err::WIN_ERROR);
				scl::err::push(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		static inline void safe_system_time_to_file_time(SYSTEMTIME &st, FILETIME &ft)
		{
			if (!SystemTimeToFileTime(&st, &ft))
			{
				scl::err::set(scl::err::WIN_ERROR);
				scl::err::push(__FILE__, __LINE__, __FUNCTION__);
			}
		}
	}
}