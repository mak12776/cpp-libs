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

		template <typename type>
		static inline type *new_array(size_t size)
		{
			return new type[size];

#ifdef SCL_CATCH_EXCEPTIONS
			try
			{
				return new type[size];
			}
			catch (std::bad_alloc exception)
			{
				error::set_error_bad_alloc_exception("unknown", size);
				return nullptr;
			}
#endif
		}

		template <typename type>
		static inline type *new_array(size_t size, type value)
		{
			type *array;

			array = new_array<type>(size);

			for (size_t index = 0; index < size; index += 1)
				array[index] = value;

			return array;

		}
	}
}