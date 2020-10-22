#pragma once

namespace winapi
{
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
	// time types

	template <typename data_type>
	struct ce_time
	{
		data_type creation, exit;

		inline ce_time<data_type> operator-(ce_time<data_type> &sub)
		{
			ce_time<data_type> res;

			res.creation = creation - sub.creation;
			res.exit = exit - sub.exit;

			return res;
		}

		inline void operator-=(ce_time<data_type> &sub)
		{
			creation -= sub.creation;
			exit -= sub.exit;
		}

		inline size_t print(FILE *stream = stdout)
		{
			size_t total_write;
			total_write = print_file_time(creation, "creation time", stream);
			total_write += print_file_time(exit, "exit time", stream);
			return total_write;
		}
	};

	template <typename data_type>
	struct ku_time
	{
		data_type kernel, user;

		inline ku_time<data_type> operator-(ku_time<data_type> &sub)
		{
			ku_time<data_type> res;

			res.kernel = kernel - sub.kernel;
			res.user = user - sub.user;

			return res;
		}

		inline void operator-=(ku_time &sub)
		{
			kernel -= sub.kernel;
			user -= sub.user;
		}

		inline size_t print(FILE *stream = stdout)
		{
			size_t total_write = 0;
			total_write += print_file_time(kernel, "kernel time", stream);
			total_write += print_file_time(user, "user time", stream);
			return total_write;
		}
	};


	template <typename data_type>
	struct ceku_time
	{
		ce_time<data_type> ce;
		ku_time<data_type> ku;

		inline size_t print(FILE *stream = stdout)
		{
			size_t total_write = 0;
			total_write += ce.print();
			total_write += ku.print();
			return total_write;
		}
	};

	// file time to system time

	static inline bool file_time_to_system_time(FILETIME &ft, SYSTEMTIME &st)
	{
		return FileTimeToSystemTime(&ft, &st);
	}

	// get times

	inline bool get_process_time(HANDLE process_handle, ceku_time<FILETIME> &time)
	{
		return GetProcessTimes(process_handle, &time.ce.creation, &time.ce.exit, &time.ku.kernel, &time.ku.user);
	}

	inline bool get_thread_time(HANDLE &thread_handle, ceku_time<FILETIME> &time)
	{
		return GetThreadTimes(thread_handle, &time.ce.creation, &time.ce.exit, &time.ku.kernel, &time.ku.user);
	}

	inline bool get_current_process_time(ceku_time<FILETIME> &time)
	{
		HANDLE process_handle = GetCurrentProcess();
		return get_process_time(process_handle, time);
	}

	inline bool get_current_thread_time(ceku_time<FILETIME> &time)
	{
		HANDLE thread_handle = GetCurrentThread();
		return get_thread_time(thread_handle, time);
	}

	// auto_timer

	struct auto_timer
	{
		ceku_time<FILETIME> start;
		ceku_time<FILETIME> end;
		typedef std::function<bool(ceku_time<FILETIME> &)> func_type;

		func_type get_now;
		bool success;

		auto_timer(func_type func_pntr = winapi::get_current_process_time)
		{
			get_now = func_pntr;
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

				(end.ku - start.ku).print();
			}
		}
	};
}