#pragma once

#include <cstdlib>
#include "error.h"

using namespace scl::error;

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
				num = ERROR_NO_MEMORY;
				set_info_size(size);
			}
			return pntr;
		}
	}
}