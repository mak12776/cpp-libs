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
			
		};
	}
}
