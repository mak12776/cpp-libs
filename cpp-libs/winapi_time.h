#pragma once

#include <Windows.h>
#include <stdio.h>
#include <functional>

#include "macros.h"
#include "clibs.h"

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


	struct ce_filetime
	{
		FILETIME creation, exit;

		inline ce_filetime operator-(ce_filetime &sub)
		{
			ce_filetime res;

			file_time_sub(creation, sub.creation, res.creation);
			file_time_sub(exit, sub.exit, res.exit);

			return res;
		}

		inline void operator-=(ce_filetime &sub)
		{
			file_time_sub(creation, sub.creation);
			file_time_sub(exit, sub.exit);
		}
	};

	struct ku_filetime
	{
		FILETIME kernel, user;

		inline ku_filetime operator-(ku_filetime &sub)
		{
			ku_filetime res;

			file_time_sub(kernel, sub.kernel, res.kernel);
			file_time_sub(user, sub.user, res.user);

			return res;
		}

		inline void operator-=(ku_filetime &sub)
		{
			file_time_sub(kernel, sub.kernel);
			file_time_sub(user, sub.user);
		}
	};

	inline int print_system_time(SYSTEMTIME &st, const char *name = nullptr, FILE *stream = stdout)
	{
		if (name == nullptr) name = "unknown";
		return fprintf(stream, "% 20s: %02u:%02u:%02u.%03u\n",
			name, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	}

	inline int print_file_time(FILETIME &ft, const char *name = nullptr, FILE *stream = stdout)
	{
		SYSTEMTIME st;
		FileTimeToSystemTime(&ft, &st);
		return print_system_time(st, name, stream);
	}

	struct ceku_filetime
	{
		ce_filetime ce;
		ku_filetime ku;

		inline ceku_filetime operator-(ceku_filetime &sub)
		{
			ceku_filetime res;
			res.ce = ce;
			res.ku = ku - sub.ku;
			return res;
		}
		inline void operator-=(ceku_filetime &sub) { ku -= sub.ku; }

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

	// System Time

	struct ce_systemtime
	{
		SYSTEMTIME creation, exit;
	};

	struct ku_systemtime
	{
		SYSTEMTIME kernel, user;
	};

	struct ceku_systemtime
	{
		ce_systemtime ce;
		ku_systemtime ku;
	};

	// functions

	static inline const char *asprint_system_time(SYSTEMTIME &st)
	{
		const char *str;
		int ret;

		ret = cl::asprintf(&str, "%02u:%02u:%02u.%03u", 
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		if (ret == -1)
			return nullptr;

		return str;
	}

	// file time to system time

	static inline bool file_time_to_system_time(FILETIME &ft, SYSTEMTIME &st)
	{
		return FileTimeToSystemTime(&ft, &st);
	}

	static inline bool file_time_to_system_time(ce_filetime &ft, ce_systemtime &st)
	{
		return 
			FileTimeToSystemTime(&ft.creation, &st.creation) &&
			FileTimeToSystemTime(&ft.exit, &st.exit);
	}

	static inline bool file_time_to_system_time(ku_filetime &ft, ku_systemtime &st)
	{
		return 
			FileTimeToSystemTime(&ft.kernel, &st.kernel) &&
			FileTimeToSystemTime(&ft.user, &st.user);
	}

	static inline bool file_time_to_system_time(
		ceku_filetime &ft, ceku_systemtime &st)
	{
		return
			file_time_to_system_time(ft.ce, st.ce) &&
			file_time_to_system_time(ft.ku, st.ku);
	}

	// get times

	inline bool get_process_time(HANDLE process_handle, ceku_filetime &time)
	{
		return GetProcessTimes(process_handle, &time.ce.creation, &time.ce.exit, &time.ku.kernel, &time.ku.user);
	}

	inline bool get_thread_time(HANDLE &thread_handle, ceku_filetime &time)
	{
		return GetThreadTimes(thread_handle, &time.ce.creation, &time.ce.exit, &time.ku.kernel, &time.ku.user);
	}

	inline bool get_current_process_time(ceku_filetime &time)
	{
		HANDLE process_handle = GetCurrentProcess();
		return get_process_time(process_handle, time);
	}

	inline bool get_current_thread_time(ceku_filetime &time)
	{
		HANDLE thread_handle = GetCurrentThread();
		return get_thread_time(thread_handle, time);
	}

	// auto_timer

	struct auto_timer
	{
		ceku_filetime start;
		ceku_filetime end;
		typedef std::function<bool(ceku_filetime &)> func_type;

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