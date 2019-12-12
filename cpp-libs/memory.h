#pragma once

#include <cstdlib>
#include "error.h"

namespace scl
{
	namespace memory
	{
		static inline void *safe_malloc(size_t size)
		{
			void *pntr;

			pntr = malloc(size);
			if (pntr == nullptr)
				error::set_error_no_memory(size);
			
			return pntr;
		}
	}
}