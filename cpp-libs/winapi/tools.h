#pragma once

namespace winapi
{
	namespace tools
	{

#ifdef SCL_EXPERIMENTAL
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