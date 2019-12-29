#pragma once

#include "memory.h"

namespace scl
{
	template <typename data_type>
	class staitc_dynamic_array
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
		bool _is_dynamic;
		pointer _data;
		size_type _capacity;
	public:
		staitc_dynamic_array(size_type capacity)
		{
			this->_is_dynamic = true;
			this->_data = new value_type[capacity];
			this->_capacity = capacity;
		}

		staitc_dynamic_array(size_type capacity, const_reference value)
		{
			this->_is_dynamic = true;
			this->_data = memory::new_array(capacity, value);
			this->_capacity = capacity;
		}

		staitc_dynamic_array(pointer data, size_type capacity)
		{
			this->_is_dynamic = false;
			this->_data = data;
			this->_capacity = capacity;
		}

		staitc_dynamic_array()
		{
			if (this->_is_dynamic)
				delete[] this->_data;
		}


	};
}