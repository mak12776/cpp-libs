#pragma once

#include "mem.h"

namespace scl
{
	template <typename data_type>
	class dynamic_array
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
		size_type _capacity;

	public:
		dynamic_array()
		{
			this->_data = nullptr;
			this->_capacity = 0;
		}

		dynamic_array(size_type capacity)
		{
			this->_data = (capacity != 0) ? mem::new_array<value_type>(capacity) : nullptr;
			this->_capacity = capacity;
		}

		dynamic_array(size_type capacity, const_reference value)
		{
			this->_capacity = capacity;
			this->_data = (capacity != 0) ? mem::new_array<value_type>(capacity, value) : nullptr;
		}

		dynamic_array(dynamic_array<data_type>& other) = delete;
		dynamic_array(dynamic_array<data_type>&& other) = delete;

		~dynamic_array()
		{
			delete[] this->_data;
		}

		// members functions

		inline bool empty() const
		{
			return this->_capacity == 0;
		}

		inline size_type size() const
		{
			return this->_capacity;
		}

		inline const_pointer data() const
		{
			return this->_data;
		}

		// access functions

		inline const_reference at(size_type length)
		{
			if constexpr (safe::error_index_out_of_range)
				safe::check_index_out_of_range(length, this->_capacity);

			return this->_data[length];
		}

		inline const_reference operator[](size_type length)
		{
			if constexpr (safe::error_index_out_of_range)
				safe::check_index_out_of_range(length, this->_capacity);

			return this->_data[length];
		}

		inline const_reference first() const
		{
			if constexpr (safe::error_zero_size)
				safe::error_zero_size(this->_capacity, "", "array test empty");

			return this->_data[0];
		}

		inline const_reference last() const
		{
			if constexpr (safe::error_zero_size)
				safe::error_zero_size(this->_capacity, "", "array test empty");

			return this->_data[this->_capacity - 1];
		}

		// algorithms

		inline void fill(const_reference value)
		{
			std::fill(this->_data, this->_data + this->_capacity, value);
		}
	};
}
