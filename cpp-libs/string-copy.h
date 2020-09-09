#pragma once

#include <string>
#include <type_traits>

#include "mem.h"

namespace scl
{
	struct c_string_t
	{
		const char *const pntr;
		const size_t size;

		c_string_t(const char *const pntr) : pntr(pntr), size(strlen(pntr) + 1)
		{ }
	};

	using mem::m_pntr;

	struct m_string_t
	{
		m_pntr<char> pntr;
		size_t size;

		m_string_t(size_t size)
		{
			pntr.alloc(size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			this->size = size;
		}
	};

	struct string_t
	{
		bool is_const;
		union
		{
			c_string_t c_string;

		};

		string_t(const char *pntr)
		{ }
		string_t(size_t size)
		{ }
	};
}
