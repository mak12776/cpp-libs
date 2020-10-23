#pragma once

namespace winapi
{
	namespace tools
	{
		// FileTime, ULARGE_INTEGER conversation
		static inline void ft_to_ul(FILETIME &ft, ULARGE_INTEGER &ul_int)
		{
			ul_int.HighPart = ft.dwHighDateTime;
			ul_int.LowPart = ft.dwLowDateTime;
		}

		static inline void ul_to_ft(ULARGE_INTEGER &ul_int, FILETIME &ft)
		{
			ft.dwHighDateTime = ul_int.HighPart;
			ft.dwLowDateTime = ul_int.LowPart;
		}

		// safe FileTime, SystemTime conversations

		static inline void safe_ft_to_st(FILETIME &ft, SYSTEMTIME &st)
		{
			if (!FileTimeToSystemTime(&ft, &st))
			{
				scl::err::set(scl::err::WIN_ERROR);
				scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		static inline void safe_st_to_ft(SYSTEMTIME &st, FILETIME &ft)
		{
			if (!SystemTimeToFileTime(&st, &ft))
			{
				scl::err::set(scl::err::WIN_ERROR);
				scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		static inline void sub_ul(ULARGE_INTEGER &ul1, ULARGE_INTEGER &ul2, ULARGE_INTEGER &res)
		{
			res.QuadPart = ul1.QuadPart - ul2.QuadPart;
		}

		static inline void sub_ul(ULARGE_INTEGER &ul1, ULARGE_INTEGER &ul2)
		{
			ul1.QuadPart = ul1.QuadPart - ul2.QuadPart;
		}

		// FILETIME overload functions

		static inline void sub_ft(FILETIME &ft1, FILETIME &ft2, FILETIME &res)
		{
			ULARGE_INTEGER t1_ul, t2_ul, res_ul;

			ft_to_ul(ft1, t1_ul);
			ft_to_ul(ft2, t2_ul);

			sub_ul(t1_ul, t2_ul, res_ul);

			ul_to_ft(res_ul, res);
		}

		inline void sub(FILETIME &ft1, FILETIME &ft2)
		{
			ULARGE_INTEGER t1_ul, t2_ul;

			ft_to_ul(ft1, t1_ul);
			ft_to_ul(ft2, t2_ul);

			sub_ul(t1_ul, t2_ul);

			ul_to_ft(t1_ul, ft1);
		}

		// print functions

		inline size_t print_system_time(SYSTEMTIME &st, const char *name, FILE *stream)
		{
			size_t size;

			if (name == nullptr) name = "unknown";
			size = scl::safe_fprintf(stream, "% 20s: %02u:%02u:%02u.%03u\n",
				name, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

			if (scl::err::check())
				scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

			return size;
		}

		inline size_t print_file_time(FILETIME &ft, const char *name, FILE *stream)
		{
			SYSTEMTIME st;
			size_t size;

			tools::safe_ft_to_st(ft, st);
			if (scl::err::check())
			{
				scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			size = print_system_time(st, name, stream);

			if (scl::err::check())
				scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

			return size;
		}

#ifdef SCL_EXPERIMENTAL
		static inline const char *asprint_system_time(SYSTEMTIME &st)
		{
			char *str;
			int ret;

			ret = cl::asprintf(&str, "%02u:%02u:%02u.%03u",
				st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

			if (ret < 0)
				return nullptr;

			return str;
		}
#endif
	}
}