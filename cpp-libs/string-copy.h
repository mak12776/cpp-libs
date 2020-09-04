#pragma once

#include <type_traits>

namespace scl
{
	struct c_string_t
	{
		const char *const pntr;
		const size_t size;

		c_string_t(const char *const pntr) : pntr(pntr), size(strlen(pntr) + 1)
		{ }
	};

	template <typename value_type>
	struct refs_count
	{
		value_type value;
		size_t refs_counts;

		refs_count() { }
		refs_count(refs_count &other) = delete;
		refs_count(refs_count &&other) = delete;
	};

	struct string_t
	{
		bool is_const;
		union
		{
			struct
			{
				const char *const pntr;
				size_t size;
			};
		};

		string_t(const char *pntr)
		{ }
		string_t(size_t size)
		{ }
	};



#if 0
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

#endif
}
