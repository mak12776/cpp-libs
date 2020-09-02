#pragma once

#include <type_traits>

namespace scl
{
	struct c_string_t
	{
		const char *const pntr;
		const size_t len;

		c_string_t(const char *pntr) : pntr(pntr), len(strlen(pntr))
		{ }
	};

	struct raw_string_t
	{
		const char *const pntr;
		const size_t len;

		raw_string_t(const char *pntr, const size_t len) : pntr(pntr), len(len)
		{ }
	};

	struct m_string_t
	{
		char *pntr;
		size_t len;

		m_string_t()
		{
			pntr = nullptr;
			len = 0;
		}

		inline void malloc_len(size_t len)
		{
			size_t size;

			math::safe_add(len, (size_t)1, size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
				return;
			}

			this->pntr = (char *)memory::safe_malloc(size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
				return;
			}
			this->len = len;
		}

		inline void malloc_len_value(size_t len, const char value)
		{
			malloc_len(len);
			if (err::check)
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
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
					err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
					return;
				}
			}

			// malloc
			this->malloc_len(len);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
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

			// malloc_len will set `this->len`
		}
	};
}
