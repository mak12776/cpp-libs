#pragma once

#include "winapi_time.h"

namespace scl
{
	namespace log
	{
		winapi::ceku_filetime start, end;

		static inline int start_process(const char *title)
		{
			winapi::get_current_process_time(start);
			return printf("%s: ", title);
		}

		static inline int end_process(const bool status)
		{
			winapi::ceku_filetime diff = end - start;
			winapi::ku_systemtime ku_systemtime;

			const char *k_str;
			const char *u_str;

			if (status)
			{
				winapi::get_current_process_time(end);
				
				winapi::file_time_to_system_time(diff.ku, ku_systemtime);

				k_str = winapi::asprint_system_time(ku_systemtime.kernel);
				if (k_str == nullptr)
					return -1;

				u_str = winapi::asprint_system_time(ku_systemtime.user);
				if (k_str == nullptr)
					return -1;

				return printf("done (%s kernel, %s user)\n", k_str, u_str);
			}
		}
	}
}