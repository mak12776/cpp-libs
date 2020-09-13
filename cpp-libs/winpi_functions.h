#pragma once

#include <Windows.h>
#include <processthreadsapi.h>

#include "err.h"

namespace winapi
{
	void safe_set_priority_class(HANDLE process_handle, DWORD Priority)
	{
		using namespace scl;

		if (!SetPriorityClass(process_handle, Priority))
		{
			err::set(err::WIN_ERROR);
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}
	}

	void safe_set_current_priority_class(DWORD periority)
	{
		HANDLE proces_handle = GetCurrentProcess();
		safe_set_priority_class(proces_handle, periority);
	}

}