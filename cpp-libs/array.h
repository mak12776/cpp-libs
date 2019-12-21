#pragma once

#include <stdlib.h>

namespace scl
{
	class fixed_array
	{
		void *pntr;
		size_t size;
	};

	class array
	{
		void *pntr;
		size_t length;
		size_t size;
	};
}