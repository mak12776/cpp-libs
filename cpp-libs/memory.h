#pragma once

#include <cstdlib>
#include <string>

#include "old_error.h"

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
#ifdef SCL_USE_ERROR
				error::set_no_memory(size);
				error::set_file_info(__FILE__, __LINE__);
#else
				throw new std::bad_alloc();
#endif
			}

			return pntr;
		}


		static inline void free(void *buffer)
		{
			std::free(buffer);
		}

		template <typename type>
		static inline type *new_array(size_t size)
		{
#ifdef SCL_USE_ERROR
			try
			{
				return new type[size];
			}
			catch (std::bad_alloc &)
			{
				error::set_no_memory_type(size, sizeof(type));
				error::set_file_info(__FILE__, __LINE__);
				return nullptr;
			}
#else
			return new type[size];
#endif
		}

		template <typename type>
		static inline type *new_array(size_t size, type value)
		{
			type *array;

			array = new_array<type>(size);
#ifdef SCL_USE_ERROR
			if (error::check()) return nullptr;
#endif

			for (size_t index = 0; index < size; index += 1)
				array[index] = value;

			return array;
		}
	}
}