#pragma once

namespace scl
{
	namespace tools
	{
		static inline bool malloc_len(char **pntr, size_t len)
		{
			

			return false;
		}

		static inline bool malloc_len_value(char **pntr, size_t len, const char value)
		{
			if (malloc_len(pntr, len))
				return true;

			memset(*pntr, value, len);
			(*pntr)[len] = '\0';
		}

		static inline bool realloc_cat()
		{
		}
	}

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
		char *_pntr;
		size_t _len;
		size_t _size;

		m_string_t()
		{
			_pntr = nullptr;
			_len = 0;
		}

		const inline size_t free_size()
		{
			return _size - _len - 1;
		}

		// malloc functions

		inline void malloc_size(size_t size)
		{
			_pntr = (char *)mem::safe_malloc(size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			_size = size;
			_len = 0;
		}

		inline void malloc_len_value(size_t len, const char value)
		{
			size_t size;

			if (len == SIZE_MAX)
			{
				err::set(err::INT_OVERFLOW);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			size = len + 1;

			_pntr = (char *)mem::safe_malloc(size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			_len = len;
			_size = size;

			memset(_pntr, value, len);
			_pntr[_len] = '\0';
		}

		// realloc functions

		inline void realloc_free_unused_space()
		{
			if (_size > _len + 1)
			{
				mem::safe_realloc(_pntr, _len + 1);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			_size = _len + 1;
		}

		inline void realloc_more_space(size_t size)
		{
			size_t new_size;

			math::safe_add(_size, size, new_size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;

			mem::safe_realloc(_pntr, new_size);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}

		inline void realloc_reverse_free_space(size_t size)
		{
			size_t free_size = this->free_size();

			if (size > free_size)
			{
				size -= free_size;

				realloc_more_space(size);
				err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		inline void realloc_cat(const c_string_t &string)
		{
			realloc_more_space(string.size);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
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
