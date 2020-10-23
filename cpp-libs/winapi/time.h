#pragma once

namespace winapi
{
	namespace time
	{
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
		};


		template <typename data_type>
		struct ceku_time
		{
			ce_time<data_type> ce;
			ku_time<data_type> ku;
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

			auto_timer(func_type func_pntr = get_current_process_time)
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
				}
			}
		};
	}
}