#pragma once

#include <stdexcept>
#include <string>

namespace scl
{
	template <typename data_type>
	class fixed_vector
	{
		typedef data_type value_type;

		typedef data_type* pointer;
		typedef const data_type* const_pointer;

		typedef data_type& reference;
		typedef const data_type& const_reference;

		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

	private:
		pointer _data;
		size_type _capacity;
		size_type _size;

	public:
		fixed_vector(size_type capacity)
		{
			this->_data = new value_type[capacity];
			this->_capacity = capacity;
			this->_size = 0;
		}

		fixed_vector(size_type capacity, size_type size, size_type value)
		{
			if (safe::check_index_out_of_range)
				safe::throw_if_out_of_range(size, capacity, nullptr, "size is greater than capacity");

			this->_data = new data_type[capacity];
			for (size_t index = 0; index < size; index += 1)
			{
				this->_data[index] = value;
			}
			this->_size = size;
		}

		fixed_vector()
		{
			delete[] this->data;
		}

		// member functions

		inline data_type *data() const
		{
			return this->data;
		}

		inline size_t capacity() const
		{
			return this->_capacity;
		}

		inline size_t size() const
		{
			return this->_size;
		}

		// access functions

		inline data_type &at(size_t index)
		{
			if (index >= this->_size)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

			return this->_data[index];
		}

		inline data_type &operator[](size_t index)
		{
			if (index >= this->_size)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

			return this->_data[index];
		}
	};
}