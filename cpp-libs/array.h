#pragma once

#include <stdlib.h>

namespace scl
{

	template <typename data_type, bool delete_on_destroy = true>
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
		dynamic_array(size_type capacity)
		{
			this->_data = new value_type[capacity];
			this->_capacity = capacity;
		}

		dynamic_array(size_type capacity, const_reference value)
		{
			this->_data = new data_type[capacity];
			for (size_type index = 0; index < capacity; index += 1)
			{
				this->_data[index] = value;
			}
			this->_capacity = capacity;
		}

		dynamic_array(pointer data, size_type capacity)
		{
			this->_data = data;
			this->_capacity = capacity;
		}

		~dynamic_array()
		{
			if (delete_on_destroy)
				delete[] this->_data;
		}

		// member functions

		inline pointer data() const
		{
			return this->data;
		}

		inline size_type size() const
		{
			return this->_capacity;
		}

		// access functions

		inline const_reference at(size_type index)
		{
			if (safe::check_index_out_of_range)
				safe::is_index_out_of_range(index, this->_capacity);

			return this->_data[index];
		}

		inline const_reference operator[](size_type index)
		{
			if (safe::check_index_out_of_range)
				safe::is_index_out_of_range(index, this->_capacity);

			return this->_data[index];
		}
	};

	template <typename data_type, size_t size>
	class array_vector : protected std::array<data_type, size>
	{
	protected:
		size_t _size;

	public:
		array_vector()
		{
			this->_size = 0;
		}

		array_vector(size_t lenght, data_type value)
		{
			if (lenght >= this->array::size())
				throw new std::out_of_range("lenght is greater than array size: " + std::to_string(lenght));

			for (size_t index = 0; index < lenght; index += 1)
			{
				this->array::_Elems[index] = value;
			}
			this->_size = lenght;
		}

		// member functions

		inline data_type *data() const
		{
			return this->_Elems;
		}

		inline size_t size() const
		{
			return this->_size;
		}

		inline constexpr size_t capacity() const
		{
			return this->array::size();
		}

		// access functions
		
		inline data_type &at(size_t index)
		{
			if (index >= this->_size)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

			return this->_Elems[index];
		}

		inline data_type &operator[](size_t index)
		{
			if (index >= this->_size)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

			return this->_Elems[index];
		}
	};
}