#pragma once

namespace scl
{
	namespace mem
	{
		typedef void *(&malloc_t)(size_t);
		typedef void *(&calloc_t)(size_t, size_t);
		typedef void *(&realloc_t)(void *, size_t);
		typedef void (&free_t)(void *);
		

		struct mem_t
		{
			malloc_t malloc;
			calloc_t calloc;
			realloc_t realloc;
			free_t free;
		};

#ifdef SCL_EXPREMENTAL
		// text logger manager
		template <log::logger_t &logger>
		void *malloc_logger(size_t size)
		{
			void *pntr = malloc(size);
			logger.printf("% 10s: %zu (%s)\n", "malloc", size
			(pntr == nullptr) ? "failed" : "success");
			return pntr;
		}

		template <log::logger_t &logger>
		void *realloc_logger(void *pntr, size_t size)
		{
			void *new_pntr = realloc(pntr, size);
			logger.printf("% 10s: %p %zu (%s)\n", "realloc", pntr, size,
				(new_pntr == nullptr) ? "failed" : "success");
			return new_pntr;
		}

		template <log::logger_t &logger>
		void free_logger(void *pntr)
		{
			printf("% 10s: %p\n", "free", pntr);
			free(pntr);
		}
#endif

		// default manager
		constexpr mem_t default_mem{ malloc, calloc, realloc, free };
		constexpr mem_t global_mem = default_mem;

		// functions

		static inline void *malloc(size_t size) 
		{ return global_mem.malloc(size); }

		static inline void *calloc(size_t nelem, size_t size)
		{ return global_mem.calloc(nelem, size); }

		static inline void *realloc(void *pntr, size_t size)
		{ return global_mem.realloc(pntr, size); }

		static inline void free(void *pntr)
		{ return global_mem.free(pntr); }

		template <typename functype, functype &func, typename... argtypes>
		static inline constexpr void *safe_call(argtypes... args)
		{
			void *pntr = func(args...);

			if (pntr == nullptr)
			{
				err::set(err::NO_MEMORY);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}

			return pntr;
		}

		static inline constexpr void *safe_malloc(size_t size)
		{ return safe_call<malloc_t, global_mem.malloc>(size); }

		static inline constexpr void *safe_calloc(size_t nelem, size_t size)
		{ return safe_call<calloc_t, global_mem.calloc>(nelem, size); }

		static inline constexpr void *safe_realloc(void *pntr, size_t size)
		{ return safe_call<realloc_t, global_mem.realloc>(pntr, size); }

#ifdef SCL_EXPERIMENTAL
		// memory pointer

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