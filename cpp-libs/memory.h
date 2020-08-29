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
				err::set(err::MALLOC);
				err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
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
			type *pntr;
#ifdef SCL_USE_ERROR
			try
			{
				pntr = new type[size];
			}
			catch (std::bad_alloc &)
			{
				err::set(err::NEW);
				err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
				return nullptr;
			}
#else
			pntr = new type[size];
#endif
			return pntr;
		}

		template <typename type>
		static inline type *new_array(size_t size, type value)
		{
			type *array;

			array = new_array<type>(size);
#ifdef SCL_USE_ERROR
			if (err::check()) return nullptr;
#endif

			for (size_t index = 0; index < size; index += 1)
				array[index] = value;

			return array;
		}
	}
}