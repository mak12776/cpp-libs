#pragma once

namespace scl
{
	namespace mem
	{
		typedef void *(&malloc_t)(size_t);
		typedef void *(&realloc_t)(void *, size_t);
		typedef void (&free_t)(void *);

		template <
			malloc_t _malloc,
			realloc_t _realloc,
			free_t _free,

			size_t _err_size,
			err::err_t<_err_size> &_err
		>
		struct mem_t
		{
			constexpr void *malloc(size_t size) 
			{ return _malloc(size); }

			constexpr void *realloc(void *pntr, size_t size) 
			{ return _realloc(pntr, size); }

			constexpr void free(void *pntr)
			{ return _free(pntr); }

			constexpr void *safe_malloc(size_t size)
			{
				void *pntr = this->malloc(size);

				if (pntr == nullptr)
				{
					_err.set(err::MALLOC);
					_err.push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				return pntr;
			}

			constexpr void *safe_realloc(void *pntr, size_t size)
			{
				void *pntr = this->realloc(pntr, size);

				if (pntr == nullptr)
				{
					_err.set(err::MALLOC);
					_err.push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				return pntr;
			}
		};

		// default manager
		typedef mem_t<
			malloc, realloc, free, err::default_array_size, err::default_err> 
			default_mem_t;
		default_mem_t default_mem;
		
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

		template <log::logger_t &logger>
		default_mem_t logger_manager{
			malloc_logger<logger>, realloc_logger<logger>, free};


		// functions

		static inline void *malloc(size_t size) 
		{
			default_mem.malloc(size);
		}

		static inline void *realloc(void *pntr, size_t size)
		{
			default_mem.realloc(pntr, size);
		}

		static inline void free(void *pntr)
		{
			default_mem.free(pntr);
		}

		static inline void *safe_malloc(size_t size)
		{
			default_mem.safe_malloc(size);
		}

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