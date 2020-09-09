#pragma once

#include "winapi_time.h"

namespace scl
{
	namespace log
	{
		winapi::ceku_time start, end;

		static inline int start_process(const char *title)
		{
			winapi::get_current_process_time(start);
			return printf("%s: ", title);
		}

		static inline int end_process(const bool status)
		{
			if (status)
			{
				winapi::get_current_process_time(end);
				return printf();
			}
		}
	}
}