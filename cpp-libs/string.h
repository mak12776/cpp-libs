#pragma once

namespace scl
{
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
			pntr = nullptr;
			len = 0;
		}

		template <const char value = '\0'>
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

			if constexpr (value != '\0')
			{
				memset(this->pntr, value, len);
				this->pntr[len] = '\0';
			}
			this->len = len;
		}

		inline void malloc_cat(c_string_t string1, c_string_t string2)
		{
			size_t len;

			math::safe_add(string1.len, string2.len, len);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
				return;
			}

			this->malloc_len(len);

			memcpy(this->pntr, string1.pntr, string1.len);
		}
	};
}
