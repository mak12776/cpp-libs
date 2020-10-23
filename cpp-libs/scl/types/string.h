#pragma once

namespace scl
{
	struct c_string_t
	{
		const char *const pntr;
		const size_t len;
		const size_t size;

		c_string_t(const char *pntr) : pntr(pntr), len(strlen(pntr)), size(len + 1)
		{ }
	};

	struct m_string_t
	{
		char *pntr;
		size_t len;
		size_t size;

		m_string_t()
		{
			pntr = nullptr;
			len = 0;
			size = 0;
		}

		const inline size_t free_size()
		{
			return size - len - 1;
		}

		// malloc functions

		inline void malloc_size(size_t _size)
		{
			pntr = (char *)mem::safe_malloc(_size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			size = _size;
			len = 0;
		}

		inline void malloc_string(const char *string)
		{
			len = strlen(string);
			size = len + 1;

			pntr = (char *)mem::safe_malloc(size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			memcpy(pntr, string, size);
		}

		inline void malloc_cat(std::initializer_list<c_string_t> list)
		{
			if (list.size() == 0)
				return;

			size_t new_size = 1;
			for (c_string_t string : list)
			{
				math::safe_add(new_size, string.len, new_size);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;
			}

			pntr = (char *)mem::safe_malloc(new_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			len = new_size - 1;
			size = new_size;

			char *new_pntr = pntr;
			for (c_string_t string : list)
			{
				memcpy(new_pntr, string.pntr, string.len);
				new_pntr += string.len;
			}
			*new_pntr = '\0';
		}

		inline void malloc_len_value(size_t length, const char value)
		{
			size_t new_size;

			if (length == SIZE_MAX)
			{
				err::set(err::INT_OVERFLOW);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			new_size = length + 1;

			pntr = (char *)mem::safe_malloc(new_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			len = length;
			size = new_size;

			memset(pntr, value, length);
			pntr[length] = '\0';
		}

		// realloc functions

		inline void realloc_free_unused_space()
		{
			if (size > len + 1)
			{
				mem::safe_realloc(pntr, len + 1);
				if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
					return;

				size = len + 1;
			}
		}

		inline void realloc_more_space(size_t more_size)
		{
			size_t new_size;

			math::safe_add(size, more_size, new_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			mem::safe_realloc(pntr, new_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			size = new_size;
		}

		inline void realloc_reverse_free_space(size_t free_size)
		{
			size_t current_free_size = this->free_size();

			if (free_size > current_free_size)
			{
				free_size -= current_free_size;

				realloc_more_space(free_size);
				err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);

				// realloc_more_space will set size.
			}
		}

		inline void realloc_cat(const c_string_t &string)
		{
			realloc_reverse_free_space(string.len);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			len += string.len;
			// realloc_free_space will set size.

			memcpy(pntr + len, string.pntr, string.size); 
			pntr[len] = '\0';
		}
	};
}
