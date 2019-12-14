#pragma once

#include <stdlib.h>

namespace scl
{
	typedef struct
	{
		void *pntr;
		size_t size;
	} fixed_array_t;

	typedef struct
	{
		void *pntr;
		size_t length;
		size_t size;
	} array_t;
}