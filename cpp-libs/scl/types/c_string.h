#pragma once

namespace scl
{
	template <typename value_type>
	struct base_c_string_t
	{
		const value_type *const pntr;
		const size_t len;

		base_c_string_t(const char *pntr) : pntr(pntr), len(strlen(pntr))
		{ }
	};

	using c_ustring_t = base_c_string_t<unsigned char>;
	using c_string_t = base_c_string_t<char>;
}