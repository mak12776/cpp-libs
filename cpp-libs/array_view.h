#pragma once

#include <stdlib.h>

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

	protected:
		pointer _data;
		size_type start;
		size_type end;


	};
}