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
			realloc_t realloc;
			free_t free;

			// unsafe malloc

			template <typename data_type>
			constexpr data_type *malloc_array(size_t size)
			{
				if (math::mul(size, sizeof data_type, size))
					return nullptr;

				return malloc(size);
			}

			// safe malloc

			constexpr void *safe_malloc(size_t size)
			{
				void *pntr = this->malloc(size);
				if (pntr == nullptr) 
					err::set_push(err::NO_MEMORY, ERR_ARGS);
				return pntr;
			}

			// safe malloc type

			template <typename data_type>
			constexpr data_type *safe_malloc_type()
			{
				data_type *pntr = (data_type *)this->malloc(sizeof data_type);
				if (pntr == nullptr)
					err::set_push(err::NO_MEMORY, ERR_ARGS);
				return pntr;
			}

			// safe malloc array

			template <typename data_type>
			constexpr data_type *safe_malloc_array(size_t size)
			{
				data_type *pntr;

				math::safe_mul(size, sizeof(data_type), size); ERR_CHECK_RETURN_VALUE(nullptr);

				pntr = (data_type *)(this->malloc(size));
				if (pntr == nullptr)
					err::set_push(err::NO_MEMORY, ERR_ARGS);

				return pntr;
			}

			// safe realloc array

			constexpr void safe_realloc(void **pntr, size_t size)
			{
				void *new_pntr = this->realloc(*pntr, size);
				if (new_pntr == nullptr)
					return err::set_push(err::NO_MEMORY, ERR_ARGS);
				*pntr = new_pntr;
			}

			template <typename data_type>
			constexpr void safe_realloc_array(data_type **pntr, size_t size)
			{
				data_type *new_pntr;

				math::safe_mul(size, sizeof(data_type), size); ERR_CHECK_RETURN;

				new_pntr = (data_type *)(this->realloc(*pntr, size));
				if (new_pntr == nullptr)
					return err::set_push(err::NO_MEMORY, ERR_ARGS);
				*pntr = new_pntr;
			}
		};

		// default manager
		constexpr mem_t default_mem{ std::malloc, std::realloc, std::free };
		mem_t global_mem = default_mem;

		// functions
		static constexpr inline void *malloc(size_t size) { return global_mem.malloc(size); } 
		static constexpr inline void *realloc(void *pntr, size_t size) { return global_mem.realloc(pntr, size); }
		static constexpr inline void free(void *pntr) { return global_mem.free(pntr); }

		template <typename data_type>
		static constexpr inline data_type *malloc_array(size_t size) { return global_mem.malloc_array(size); }

		static constexpr inline void *safe_malloc(size_t size) { return global_mem.safe_malloc(size); }
		static constexpr inline void safe_realloc(void **pntr, size_t size) { return global_mem.safe_realloc(pntr, size); }

		template <typename data_type>
		static constexpr inline data_type *safe_malloc_type() { return global_mem.safe_malloc_type<data_type>(); }

		template <typename data_type>
		static constexpr inline data_type *safe_malloc_array(size_t size) { return global_mem.safe_malloc_array<data_type>(size); }

		template <typename data_type>
		static constexpr inline void safe_realloc_array(data_type **pntr, size_t size) { return global_mem.safe_realloc_array(pntr, size); }

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
				err::push(__FILE__, __LINE__, __FUNCTION__);
				return nullptr;
			}
#else
			pntr = new type[size];
#endif
			return pntr;
		}
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
					err::push(__FILE__, __LINE__, __FUNCTION__);
					return;
				}
				this->refs = 1;
			}
		};
#endif
	}
}