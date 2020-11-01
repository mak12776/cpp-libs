#pragma once

namespace scl
{
	template <typename data_type>
	struct sorted_vector_t
	{
		data_type *data;
		size_t len;
		size_t size;

		sorted_vector_t()
			: data(nullptr), len(0), size(0)
		{ }

		sorted_vector_t(data_type *data, size_t len, size_t size)
			: data(data), len(len), size(size)
		{ }

		inline void safe_malloc_size(size_t size)
		{

		}
	};
}