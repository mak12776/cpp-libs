#pragma once

namespace scl
{
	namespace fs
	{
		// file system separator

#if defined(_WIN32) || defined(_WIN64)
		const char *sep = "\\";
#elif defined(__linux__)
		const char *sep = "/";
#else
#error Unknown operating system
#endif
	}
}