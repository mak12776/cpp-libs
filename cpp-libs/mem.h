#pragma once

#include <cstdlib>
#include <string>
#include "err.h"

namespace scl
{
	namespace mem
	{
		struct manager
		{
			void *(&malloc)(size_t);
			void *(&realloc)(void *, size_t);
			void(&free)(void *);

			manager(void *(&malloc)(size_t), void *(&realloc)(void *, size_t), void(&free)(void *))
				: malloc(malloc), realloc(realloc), free(free)
			{ }
		};

		manager default_manager(malloc, realloc, free);

		template <typename value_type>
		struct m_pntr
		{
			value_type *pntr;
			size_t refs;

			inline void alloc(size_t size)
			{
				this->pntr = mem::safe_malloc(size);
				if (err::check())
				{
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
					return;
				}
				this->refs = 1;
			}
		};

		static inline void *safe_malloc(size_t size)
		{
			void *pntr;

			pntr = malloc(size);

			if (pntr == nullptr)
			{
				err::set(err::MALLOC);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
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
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
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

			if (err::check())
				return nullptr;

			for (type *pntr = array, *end = array + size; pntr < end; pntr += 1)
				(*pntr) = value;

			return array;
		}
	}
}