#pragma once

namespace scl
{
	namespace mem
	{
		typedef void *(*malloc_t)(size_t);
		typedef void *(*calloc_t)(size_t, size_t);
		typedef void *(*realloc_t)(void *, size_t);
		typedef void (*free_t)(void *);

		struct mem_t
		{
			malloc_t malloc;
			free_t free;
			calloc_t calloc;
			realloc_t realloc;

			constexpr void *safe_malloc(size_t size)
			{
				void *pntr = this->malloc(size);
				if (pntr == nullptr)
				{
					err::set(err::NO_MEMORY);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				return pntr;
			}

			constexpr void *safe_calloc(size_t count, size_t size)
			{
				void *pntr = this->calloc(count, size);
				if (pntr == nullptr)
				{
					err::set(err::NO_MEMORY);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				return pntr;
			}

			// safe malloc array

			template <typename data_type>
			constexpr data_type *safe_malloc_array(size_t size)
			{
				data_type *pntr;

				math::safe_mul(size, sizeof(data_type), size);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return nullptr;

				pntr = (data_type *)this->malloc(size);

				if (pntr == nullptr)
				{
					err::set(err::NO_MEMORY);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				printf("safe_malloc_array<%s: %zu>(%zu): %p\n", typeid(data_type).name(), sizeof(data_type), size, pntr);

				return pntr;
			}

			template <typename data_type>
			constexpr void *safe_malloc_array(size_t size, const data_type &data)
			{
				data_type *pntr = safe_malloc_array<data_type>(size);

				if (not err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					std::fill_n(pntr, size, data);

				return pntr;
			}

			// safe realloc array

			constexpr void *safe_realloc(void *pntr, size_t size)
			{
				void *new_pntr = this->realloc(pntr, size);

				if (new_pntr == nullptr)
				{
					err::set(err::NO_MEMORY);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				return new_pntr;
			}

			template <typename data_type>
			constexpr data_type *safe_realloc_array(data_type *pntr, size_t size)
			{
				data_type *new_pntr = nullptr;

				printf("safe_realloc_array<%s: %zu>(%p, %zu) = ", typeid(data_type).name(), sizeof(data_type), pntr, size);


				math::safe_mul(size, sizeof(data_type), size);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return nullptr;

				new_pntr = (data_type *)this->realloc(pntr, size);

				if (new_pntr == nullptr)
				{
					err::set(err::NO_MEMORY);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				printf("%p\n", new_pntr);

				return new_pntr;
			}
		};

		// default manager
		constexpr mem_t default_mem{ &std::malloc, &std::calloc, &std::realloc, &std::free };
		mem_t global_mem = default_mem;

		// functions

#ifdef SCL_EXPERIMENTAL
		static constexpr inline void *malloc(size_t size) { return global_mem.malloc(size); } 
		static constexpr inline void *calloc(size_t nelem, size_t size) { return global_mem.calloc(nelem, size); }
		static constexpr inline void *realloc(void *pntr, size_t size) { return global_mem.realloc(pntr, size); }
		static constexpr inline void free(void *pntr) { return global_mem.free(pntr); }
#endif // SCL_EXPERIMENTAL

		static constexpr inline void *safe_malloc(size_t size) { return global_mem.safe_malloc(size); }
		static constexpr inline void *safe_calloc(size_t count, size_t size) { return global_mem.safe_calloc(count, size); }
		static constexpr inline void *safe_realloc(void *pntr, size_t size) { return global_mem.safe_realloc(pntr, size); }

		template <typename data_type>
		static constexpr inline data_type *safe_malloc_array(size_t size) { return global_mem.safe_malloc_array<data_type>(size); }

		template <typename data_type>
		static constexpr inline data_type *safe_realloc_array(data_type *pntr, size_t size) { return global_mem.safe_realloc_array(pntr, size); }

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
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
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
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
					return;
				}
				this->refs = 1;
			}
		};
#endif
	}
}