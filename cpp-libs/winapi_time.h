#pragma once

#include <Windows.h>
#include <stdio.h>
#include <functional>

#include "macros.h"

namespace winapi
{
	// file time conversation

	inline void file_time_to_ularge_integer(FILETIME &ft, ULARGE_INTEGER &ul_int)
	{
		ul_int.HighPart = ft.dwHighDateTime;
		ul_int.LowPart = ft.dwLowDateTime;
	}

	inline void ularge_integer_to_file_time(ULARGE_INTEGER &ul_int, FILETIME &ft)
	{
		ft.dwHighDateTime = ul_int.HighPart;
		ft.dwLowDateTime = ul_int.LowPart;
	}

	// res_ft = ft1 - ft2;
	inline void file_time_sub(FILETIME &ft1, FILETIME &ft2, FILETIME &res_ft)
	{
		ULARGE_INTEGER t1_ul, t2_ul, res_ul;

		file_time_to_ularge_integer(ft1, t1_ul);
		file_time_to_ularge_integer(ft2, t2_ul);

		res_ul.QuadPart = t1_ul.QuadPart - t2_ul.QuadPart;

		ularge_integer_to_file_time(res_ul, res_ft);
	}

	// ft1 = ft1 - ft2;
	inline void file_time_sub(FILETIME &ft1, FILETIME &ft2)
	{
		ULARGE_INTEGER t1_ul, t2_ul;

		file_time_to_ularge_integer(ft1, t1_ul);
		file_time_to_ularge_integer(ft2, t2_ul);

		t1_ul.QuadPart = t1_ul.QuadPart - t2_ul.QuadPart;

		ularge_integer_to_file_time(t1_ul, ft1);
	}


	struct ce_time
	{
		FILETIME creation, exit;

		inline ce_time operator-(ce_time &sub)
		{
			ce_time res;

			file_time_sub(creation, sub.creation, res.creation);
			file_time_sub(exit, sub.exit, res.exit);

			return res;
		}

		inline void operator-=(ce_time &sub)
		{
			file_time_sub(creation, sub.creation);
			file_time_sub(exit, sub.exit);
		}
	};

	struct ku_time
	{
		FILETIME kernel, user;

		inline ku_time operator-(ku_time &sub)
		{
			ku_time res;

			file_time_sub(kernel, sub.kernel, res.kernel);
			file_time_sub(user, sub.user, res.user);

			return res;
		}

		inline void operator-=(ku_time &sub)
		{
			file_time_sub(kernel, sub.kernel);
			file_time_sub(user, sub.user);
		}
	};

	inline size_t print_system_time(SYSTEMTIME &st, const char *name = nullptr, FILE *stream = stdout)
	{
		if (name == nullptr) name = "unknown";
		return fprintf(stream, "% 20s: %02u:%02u:%02u.%03u\n", 
			name, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	}

	inline size_t print_file_time(FILETIME &ft, const char *name = nullptr, FILE *stream = stdout)
	{
		SYSTEMTIME st;
		FileTimeToSystemTime(&ft, &st);
		return print_system_time(st, name, stream);
	}

	struct ceku_time
	{
		ce_time ce;
		ku_time ku;

		inline ceku_time operator-(ceku_time &sub)
		{
			ceku_time res;
			res.ce = ce;
			res.ku = ku - sub.ku;
			return res;
		}
		inline void operator-=(ceku_time &sub) { ku -= sub.ku; }

		inline size_t print(FILE *stream = stdout)
		{
			size_t total_write;
			total_write = print_file_time(ce.creation, "creation time", stream);
			total_write += print_file_time(ce.exit,	"exit time", stream);
			total_write += print_file_time(ku.kernel, "kernel time", stream);
			total_write += print_file_time(ku.user,	"user time", stream);
			return total_write;
		}
	};

	// get times

	inline bool get_process_time(HANDLE process_handle, ceku_time &time)
	{
		return GetProcessTimes(process_handle, &time.ce.creation, &time.ce.exit, &time.ku.kernel, &time.ku.user);
	}

	inline bool get_thread_time(HANDLE &thread_handle, ceku_time &time)
	{
		return GetThreadTimes(thread_handle, &time.ce.creation, &time.ce.exit, &time.ku.kernel, &time.ku.user);
	}

	inline bool get_current_process_time(ceku_time &time)
	{
		HANDLE process_handle = GetCurrentProcess();
		return get_process_time(process_handle, time);
	}

	inline bool get_current_thread_time(ceku_time &time)
	{
		HANDLE thread_handle = GetCurrentThread();
		return get_thread_time(thread_handle, time);
	}

	// auto_timer

	struct auto_timer
	{
		ceku_time start;
		ceku_time end;
		typedef std::function<bool(ceku_time &)> func_type;

		func_type get_now;
		bool success;

		auto_timer(func_type func = winapi::get_current_process_time)
		{
			get_now = func;
			if (!get_now(start))
			{
				printf(__FILE__ ":" STR(__LINE__) ": get start time failed.");
				success = false;
			}
			success = true;
		}

		~auto_timer()
		{
			if (success)
			{
				if (!get_now(end))
				{
					printf(__FILE__ ":" STR(__LINE__) ": get end time failed.");
					return;
				}

				(end - start).print();
			}
		}
	};
}