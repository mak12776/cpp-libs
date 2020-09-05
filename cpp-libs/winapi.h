#pragma once

#include "winapi_types.h"

#include <Windows.h>

namespace winapi
{
	inline void file_time_to_ularge_integer(FILETIME *ft, ULARGE_INTEGER *ul_int)
	{
		ul_int->HighPart = ft->dwHighDateTime;
		ul_int->LowPart = ft->dwLowDateTime;
	}

	inline void ularge_integer_to_file_time(ULARGE_INTEGER *ul_int, FILETIME *ft)
	{
		ul_int->HighPart = ft->dwHighDateTime;
		ul_int->LowPart = ft->dwLowDateTime;
	}

	inline void file_time_sub(FILETIME *t1, FILETIME *t2, FILETIME *result)
	{
		ULARGE_INTEGER t1_int;
		ULARGE_INTEGER t2_int;
		ULARGE_INTEGER t3_int;

		file_time_to_ularge_integer(t1, &t1_int);
		file_time_to_ularge_integer(t1, &t1_int);

		t3_int.QuadPart = t1_int.QuadPart - t2_int.QuadPart;

		ularge_integer_to_file_time(&t3_int, result);
	}
}