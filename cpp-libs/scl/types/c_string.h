#pragma once

namespace scl
{
	template <typename value_type>
	struct c_base_string_t
	{
		const value_type *const pntr;
		const size_t len;

		c_base_string_t(const char *pntr) : pntr(pntr), len(strlen(pntr))
		{ }
	};

	using c_ustring_t = c_base_string_t<unsigned char>;
	using c_string_t = c_base_string_t<char>;
}