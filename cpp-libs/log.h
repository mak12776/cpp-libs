#pragma once

#include "winapi_time.h"

namespace scl
{
	namespace log
	{
		winapi::ceku_time<FILETIME> start, end;

		static inline int start_process(const char *title)
		{
			winapi::get_current_process_time(start);
			return printf("%s: ", title);
		}

		static inline int end_process(const bool status)
		{
			winapi::ceku_time<FILETIME> diff;
			SYSTEMTIME kernel_st;
			SYSTEMTIME user_st;

			const char *k_str;
			const char *u_str;

			if (status)
			{
				winapi::get_current_process_time(end);
				
				winapi::file_time_to_system_time(diff.ku.kernel, kernel_st);
				winapi::file_time_to_system_time(diff.ku.user, user_st);

				k_str = winapi::asprint_system_time(kernel_st);
				if (k_str == nullptr)
					return -1;

				u_str = winapi::asprint_system_time(user_st);
				if (k_str == nullptr)
					return -1;

				int ret = printf("done (%s kernel, %s user)\n", k_str, u_str);
				if (start.ku.user.dwLowDateTime == end.ku.user.dwLowDateTime)
					cl::printf_ln("equal");
				else
					cl::printf_ln("not equal");

				start.ku.print();
				end.ku.print();
				(end.ku - start.ku).print();

				return ret;
			}
			return 0;
		}
	}
}