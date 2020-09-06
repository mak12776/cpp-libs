#pragma once

#include <Windows.h>
#include <stdio.h>
#include "winapi_tools.h"

namespace winapi
{
	inline void file_time_to_ularge_integer(FILETIME *ft, ULARGE_INTEGER *ul_int)
	{
		ul_int->HighPart = ft->dwHighDateTime;
		ul_int->LowPart = ft->dwLowDateTime;
	}

	inline void ularge_integer_to_file_time(ULARGE_INTEGER *ul_int, FILETIME *ft)
	{
		ft->dwHighDateTime = ul_int->HighPart;
		ft->dwLowDateTime = ul_int->LowPart;
	}
	
	inline void file_time_sub(FILETIME *t1, FILETIME *t2, FILETIME *result)
	{
		ULARGE_INTEGER t1_ul;
		ULARGE_INTEGER t2_ul;
		ULARGE_INTEGER result_ul;

		file_time_to_ularge_integer(t1, &t1_ul);
		file_time_to_ularge_integer(t2, &t2_ul);

		result_ul.QuadPart = t1_ul.QuadPart - t2_ul.QuadPart;

		ularge_integer_to_file_time(&result_ul, result);
	}

	size_t printf_system_time(SYSTEMTIME &system_time)
	{

		// d: day, h: hour, m: minute, s: second, i: milli second
		return printf("%02u:%02u:%02u.%03u\n", system_time.wHour, system_time.wMinute, system_time.wSecond, system_time.wMilliseconds);
	}

	size_t printf_file_time(FILETIME &ft)
	{
		SYSTEMTIME st;

		FileTimeToSystemTime(&ft, &st);
		return printf_system_time(st);
	}

	struct creation_exit_time
	{
		FILETIME creation, exit;
	};

	struct kernel_user_time
	{
		FILETIME kernel, user;
	};

	struct all_times
	{
		struct creation_exit_time;
		struct kernel_user_time;
	};

	struct process_time_t
	{
		FILETIME creation_ft, exit_time, kernel_time, user_time;

		inline process_time_t &operator-(process_time_t &other)
		{
			process_time_t result;

			result.creation_ft = other.creation_ft;
			result.exit_time = other.exit_time;

			winapi::file_time_sub(&this->kernel_time, &other.kernel_time, &result.kernel_time);
			winapi::file_time_sub(&this->user_time, &other.user_time, &result.user_time);

			return result;
		}

		size_t print(FILE *stream = stdout)
		{
			size_t total_write = 0;

			total_write += printf_file_time(this->creation_ft);
			total_write += printf_file_time(this->kernel_time);
			total_write += printf_file_time(this->user_time);

			return total_write;
		}
	};

	inline bool get_process_time(HANDLE process_handle, process_time_t &time)
	{
		return GetProcessTimes(process_handle, &time.creation_ft, &time.exit_time, &time.kernel_time, &time.user_time);
	}

	inline bool get_thread_time(HANDLE &thread_handle, process_time_t &time)
	{
		return GetThreadTimes(thread_handle, &time.creation_ft, &time.exit_time, &time.kernel_time, &time.user_time);
	}

	inline bool get_current_process_time(process_time_t &time)
	{
		HANDLE process_handle = GetCurrentProcess();
		return get_process_time(process_handle, time);
	}

	inline bool get_current_thread_time(process_time_t &time)
	{
		HANDLE thread_handle = GetCurrentThread();
		return get_thread_time(thread_handle, time);
	}
}