#pragma once

#include <cstdlib>
#include <string>
#include "err.h"

#include "log.h"

namespace scl
{
	namespace mem
	{
		typedef void *(&malloc_t)(size_t);
		typedef void *(&realloc_t)(void *, size_t);
		typedef void (&free_t)(void *);

		template <logger_t &logger>
		void *malloc_logger(size_t size)
		{
			void *pntr = malloc(size);
			logger.printf("% 10s: %zu (%s)\n", "malloc", 
				size, (pntr == nullptr) ? "failed" : "success");
			return pntr;
		}

		template <logger_t &logger>
		void *realloc_logger(void *pntr, size_t size)
		{
			void *new_pntr = realloc(pntr, size);
			logger.printf("% 10s: %zu (%s)\n", "realloc",
				size, (new_pntr == nullptr) ? "failed" : "success");
			return new_pntr;
		}

		struct manager_t
		{
			malloc_t malloc;
			realloc_t realloc;
			free_t free;

			inline void *safe_malloc(size_t size)
			{
				void *pntr = this->malloc(size);

				if (pntr == nullptr)
				{
					err::set(err::MALLOC);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				return pntr;
			}
		};

		manager_t default_manager{ malloc, realloc, free };
		manager_t logger_manager{ 
			malloc_logger<default_logger>, realloc_logger<default_logger>, free};

#ifdef SCL_EXPERIMENTAL
		template <typename value_type, intptr_t manager = default_manager_pntr>
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
#endif

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