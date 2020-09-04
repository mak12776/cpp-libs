#pragma once

#include <time.h>
#include <stdio.h>

namespace scl
{
	namespace timer
	{
		struct auto_timer
		{
			clock_t start;
			clock_t end;

			auto_timer()
			{
				start = clock();
			}

			auto_timer(auto_timer &other) = delete;
			auto_timer(auto_timer &&other) = delete;

			~auto_timer()
			{
				end = clock();

				printf("%lu, %lu\n", start, end);
				printf("Wall clock: %f\n", (double)(end - start));
			}
		};
	}
}
