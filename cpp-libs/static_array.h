#pragma once

#include <stdlib.h>

namespace scl
{
	template <typename data_type, size_t _size>
	class static_array
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

	public:
		static_array(pointer data)
		{
			this->_data = data;
		}

		// members functions

		inline size_type empty() const
		{
			return this->_size == 0;
		}

		inline constexpr size_type size() const
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
			if (safe::check_index_out_of_range)
				safe::throw_if_out_of_range(index, _size);

			return this->_data[index];
		}

		inline const_reference operator[](size_type index)
		{
			if (safe::check_index_out_of_range)
				safe::throw_if_out_of_range(index, _size);

			return this->_data[index];
		}

		inline const_reference first() const
		{
			if (safe::check_zero_size)
				safe::check_zero_size(_size, "", "array is empty");

			return this->_data[0];
		}

		inline const_reference last() const
		{
			if (safe::check_zero_size)
				safe::check_zero_size(_size, "", "array is empty");

			return this->_data[_size - 1];
		}

		// operations

		inline void fill(const_reference value)
		{
			std::fill(this->_data, this->_data + _size, value);
		}
	};
}