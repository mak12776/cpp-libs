#pragma once

#include "dynamic_array.h"

namespace scl
{
	template <typename data_type, bool delete_on_destroy = false>
	class array_view
	{

	public:
		typedef data_type value_type;

		typedef data_type* pointer;
		typedef const data_type* const_pointer;

		typedef data_type& reference;
		typedef const data_type& const_reference;

		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

	public:
		pointer data;
		size_type start;
		size_type end;

		array_view(pointer data, size_type start, size_type end)
			: data(data), start(start), end(end)
		{ }

		template <bool _delete_on_destroy>
		array_view(dynamic_array<data_type, _delete_on_destroy> array)
		{
			this->data = array->data();
			this->start = 0;
			this->end = 0;
		}
	};
}