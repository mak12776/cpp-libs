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
			winapi::ceku_filetime diff;
			winapi::ku_systemtime ku;

			if (status)
			{
				winapi::get_current_process_time(end);
				
				winapi::file_time_to_system_time(diff.ku, ku);

				return printf("done (%.3f kernel, %.3f user)\n", 
					(double)kernel_ul.QuadPart);
				
			}
		}
	}
}