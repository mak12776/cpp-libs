#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#elif defined(__linux__)
#error include necessary headers.
#endif

#include <time.h>
#include <stdio.h>

namespace scl
{
	namespace timer
	{
		struct auto_timer
		{
			time_t start;
			time_t end;
			bool get_process_times_success = true;

			auto_timer()
			{
				if (scl::get_now(&start))
				{
					printf("error: GetProcessTimes Failed on start.\n");
					get_process_times_success = false;
					return;
				}
			}

			auto_timer(auto_timer &other) = delete;
			auto_timer(auto_timer &&other) = delete;

			~auto_timer()
			{
				if (get_process_times_success)
				{
					if (scl::get_now(&end))
					{
						printf("error: GetProcessTimes Failed on end.\n");
						return;
					}

				}
			}
		};
	}
}
