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
				safe::throw_if_out_of_range(index, this->_capacity);

			return this->_data[index];
		}

		inline const_reference operator[](size_type index)
		{
			if (safe::check_index_out_of_range)
				safe::throw_if_out_of_range(index, this->_capacity);

			return this->_data[index];
		}
	};

	template <typename data_type, size_t size>
	class array_vector : protected std::array<data_type, size>
	{
		typedef data_type value_type;

		typedef value_type* pointer;
		typedef const value_type* const_pointer;

		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference;

	protected:
		size_t _lenght;

	public:
		array_vector()
		{
			this->_lenght = 0;
		}

		array_vector(size_type lenght, const_reference value)
		{
			if (safe::check_index_out_of_range())
				safe::throw_if_out_of_range(lenght, this->array::size(), "", "lenght is greater than array size");

			for (size_t index = 0; index < lenght; index += 1)
			{
				this->array::_Elems[index] = value;
			}
			this->_lenght = lenght;
		}

		// member functions

		inline pointer data() const
		{
			return this->_Elems;
		}

		inline size_type lenght() const
		{
			return this->_lenght;
		}

		inline constexpr size_type size() const
		{
			return this->array::size();
		}

		// access functions
		
		inline const_reference at(size_type index)
		{
			if (safe::check_index_out_of_range)
				safe::throw_if_out_of_range(index, this->_lenght);

			return this->_Elems[index];
		}

		inline const_reference operator[](size_type index)
		{
			if (safe::check_index_out_of_range)
				safe::throw_if_out_of_range(index, this->_lenght);

			return this->_Elems[index];
		}
	};
}