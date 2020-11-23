#pragma once

namespace scl
{
	template <typename value_type>
	struct base_c_string_t
	{
		const value_type *const pntr;
		const size_t len;

		template <size_t size>
		base_c_string_t(const value_type(&pntr)[size]) : pntr(pntr), len(size - 1)
		{ }
	};

	using c_ustring_t = base_c_string_t<unsigned char>;
	using c_string_t = base_c_string_t<char>;
}