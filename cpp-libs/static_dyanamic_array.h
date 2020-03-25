#pragma once

#include "memory.h"

namespace scl
{
	template <typename data_type>
	class static_dynamic_array
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
		bool _is_dynamic;

	public:
		static_dynamic_array(size_type capacity)
		{
			this->_is_dynamic = true;
			this->_data = new value_type[capacity];
			this->_capacity = capacity;
		}

		static_dynamic_array(size_type capacity, const_reference value)
		{
			this->_is_dynamic = true;
			this->_data = memory::new_array<value_type>(capacity, value);
			this->_capacity = capacity;
		}

		static_dynamic_array(pointer data, size_type capacity)
		{
			this->_is_dynamic = false;
			this->_data = data;
			this->_capacity = capacity;
		}

		static_dynamic_array(static_dynamic_array<data_type>& other) = delete;
		static_dynamic_array(static_dynamic_array<data_type>&& other) = delete;

		~static_dynamic_array()
		{
			if (this->_is_dynamic)
				delete[] this->_data;
		}

		// members functions

		inline const bool is_dynamic() const
		{
			return this->_is_dynamic;
		}

		inline const bool is_static() const
		{
			return !(this->_is_dynamic);
		}

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

		inline const_reference at(size_type index)
		{
			if constexpr (safe::error_index_out_of_range)
				safe::check_index_out_of_range(index, this->_capacity);

			return this->_data[index];
		}

		inline const_reference operator[](size_type index)
		{
			if constexpr (safe::error_index_out_of_range)
				safe::check_index_out_of_range(index, this->_capacity);

			return this->_data[index];
		}

		inline const_reference first() const
		{
			if constexpr (safe::error_zero_size)
				safe::check_zero_size(this->_capacity, "", "array is empty");

			return this->_data[0];
		}

		inline const_reference last() const
		{
			if constexpr (safe::error_zero_size)
				safe::check_zero_size(this->_capacity, "", "array is empty");

			return this->_data[this->_capacity - 1];
		}

		// algorithms

		inline void fill(const_reference value)
		{
			std::fill(this->_data, this->_data + this->_capacity, value);
		}
	};
}