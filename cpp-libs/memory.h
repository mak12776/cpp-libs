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
			{
				error::num = error::ERROR_NO_MEMORY;
				error::set_info_size(size);
			}
			return pntr;
		}
	}
}