#pragma once

#include <cstdlib>

namespace scl
{
	template <typename data_type, size_t _size>
	class array_pointer
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
		const_pointer _data;

	public:
		array_pointer(const_pointer data)
		{
			this->_data = data;
		}

		array_pointer(const_pointer data, const_reference value)
		{
			this->_data = data;
			std::fill(data, data + _size, value);
		}

		array_pointer(array_pointer<value_type, _size>& other) = delete;
		array_pointer(array_pointer<value_type, _size>&& other) = delete;

		// members functions

		inline size_type empty() const
		{
			return this->_size == 0;
		}

		inline size_type size() const
		{
			return this->_size;
		}

		inline const_pointer data() const
		{
			return this->_data;
		}

		// access functions

		inline const_reference at(size_type index)
		{
			if (safe::error_index_out_of_range)
				safe::check_index_out_of_range(index, _size);

			return this->_data[index];
		}

		inline const_reference operator[](size_type index)
		{
			if (safe::error_index_out_of_range)
				safe::check_index_out_of_range(index, _size);

			return this->_data[index];
		}

		inline const_reference first() const
		{
			if (safe::error_zero_size)
				safe::error_zero_size(_size, "", "array is empty");

			return this->_data[0];
		}

		inline const_reference last() const
		{
			if (safe::error_zero_size)
				safe::error_zero_size(_size, "", "array is empty");

			return this->_data[_size - 1];
		}

		// algorithms

		inline void fill(const_reference value)
		{
			std::fill(this->_data, this->_data + _size, value);
		}
	};
}