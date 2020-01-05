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
		static inline type *new_array(size_t size, type value)
		{
			type *array;

			array = new type[size];

			for (size_t index = 0; index < size; index += 1)
				array[index] = value;

			return array;
		}
	}
}