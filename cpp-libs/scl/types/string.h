#pragma once

namespace scl
{
	namespace tools
	{
		static inline bool malloc_len(char **pntr, size_t len)
		{
			size_t size;

			math::safe_add(len, (size_t)1, len);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return true;
			}

			(*pntr) = (char *)mem::safe_malloc(size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return true;
			}

			return false;
		}

		static inline bool malloc_len_value(char **pntr, size_t len, const char value)
		{
			if (malloc_len(pntr, len))
				return true;

			memset(*pntr, value, len);
			(*pntr)[len] = '\0';
		}

		static inline bool malloc_cat()
		{

		}
	}

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
			if (tools::malloc_len(&(this->pntr), len))
				return;
			this->len = len;
		}

		inline void malloc_len_value(size_t len, const char value)
		{
			if (tools::malloc_len_value(&(this->pntr), len, value))
				return;
			this->len = len;
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

	struct sring_t
	{
		char *pntr;

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
