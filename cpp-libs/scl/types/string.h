#pragma once

namespace scl
{

#ifdef SCL_EXPERIMENTAL
	namespace string_tools
	{
		static inline char *malloc_len(size_t len)
		{
			char *pntr;
			size_t size;

			math::safe_add(len, (size_t)1, size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return nullptr;
			}

			pntr = (char *)mem::safe_malloc(size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return nullptr;
			}

			return pntr;
		}

		static inline char *malloc_len_value(size_t len, const char value)
		{
			char *pntr;
			
			pntr = malloc_len(len);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
				return nullptr;
			}
			memset((void *)pntr, 0, len);
			pntr[len] = '\0';

			return pntr;
		}
	}
#endif

	struct c_string_t
	{
		const char *const pntr;
		const size_t len;

		c_string_t(const char *pntr) : pntr(pntr), len(strlen(pntr))
		{ }
	};

	struct m_string_t
	{
		char *pntr;
		size_t len;

		m_string_t()
		{
			this->pntr = nullptr;
			this->len = 0;
		}

		inline void malloc_len(size_t len)
		{
			size_t size;

			math::safe_add(len, (size_t)1, size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			this->pntr = (char *)mem::safe_malloc(size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			this->len = len;
		}

		inline void malloc_len_value(size_t len, const char value)
		{
			malloc_len(len);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			memset(this->pntr, value, len);
			this->pntr[len] = '\0';
		}

		inline void malloc_cat(const std::initializer_list<c_string_t> &list)
		{
			size_t len;

			if (list.size() == 0)
				return;

			// sum total len
			auto iter = list.begin();
			len = iter->len;

			while ((++iter) != list.end())
			{
				math::safe_add(len, iter->len, len);
				if (err::check())
				{
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
					return;
				}
			}

			// malloc
			this->malloc_len(len);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			// copy the strings
			iter = list.begin();
			char *c_pntr = this->pntr;

			memcpy(c_pntr, iter->pntr, iter->len);
			c_pntr += iter->len;
			 
			while ((++iter) != list.end())
			{
				memcpy(c_pntr, iter->pntr, iter->len);
				c_pntr += iter->len;
			}
			*c_pntr = '\0';
			// malloc_len set `this->len`
		}
	};

#ifdef SCL_EXPERIMENTAL
	struct string_t
	{
		bool is_const;
		char *pntr;
		size_t len;

		string_t(const char *pntr)
		{
			this->is_const = true;
			this->pntr = (char *)pntr;
			this->len = strlen(pntr) + 1; // it's safe to add 1
		}

		string_t()
		{
			is_const = false;
			pntr = nullptr;
			len = 0;
		}

		string_t(size_t len)
		{

		}
	};
#endif

}
