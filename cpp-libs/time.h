#pragma once

#include <windows.h>
#include "winapi.h"

namespace scl
{
	struct time_t
	{

#if defined(_WIN64) || defined(_WIN32)
		FILETIME creation_time, exit_time, kernel_time, user_time;
#elif defined(__linux__)
#error incomplete code for linux platform
#endif

		inline time_t &operator-(time_t &other)
		{
			time_t result;

#if defined(_WIN32) || defined(_WIN64)
			result.creation_time = other.creation_time;
			result.exit_time = other.exit_time;
			winapi::file_time_sub(&this->kernel_time, &other.kernel_time, &result.kernel_time);
			winapi::file_time_sub(&this->user_time, &other.user_time, &result.user_time);
#elif defined(__linux__)
#error incomplete code for linux platform
#endif

			return result;
		}
	};

	inline bool get_now(time_t *time)
	{
#if defined(_WIN64) || defined(_WIN32)
		HANDLE process_handle = GetCurrentProcess();

		return GetProcessTimes(process_handle, &time->creation_time, &time->exit_time, &time->kernel_time, &time->user_time) == 0;
#elif defined(__linux__)
#error incomplete code section for linux platform
#endif
	}
}
