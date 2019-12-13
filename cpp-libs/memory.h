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

		template <typename T>
		static inline T *new_array(size_t size)
		{
#ifdef SCL_CATCH_EXCEPTIONS
			try
			{
				return new T[size];
			}
			catch (std::bad_alloc exception)
			{
				error::set_error_bad_alloc_exception("unknown", size);
				return nullptr;
			}
#else
			return new T[size];
#endif
		}
	}
}