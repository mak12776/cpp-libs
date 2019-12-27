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
			this->_data = (capacity != 0) ? new value_type[capacity] : nullptr;
			this->_capacity = capacity;
		}

		dynamic_array(size_type capacity, const_reference value)
		{
			if (capacity != 0)
			{
				this->_data = new data_type[capacity];
				fill(value);
			}
			else
				this->data = nullptr;

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

		// access functions

		inline const_pointer data() const
		{
			return this->_data;
		}

		inline const_reference first() const
		{
			return this->_data[0];
		}

		inline const_reference last() const
		{
			return this->_data[0];
		}

		// size functions

		inline size_type empty() const
		{
			return this->_capacity == 0;
		}

		inline size_type size() const
		{
			return this->_capacity;
		}

		// operations

		inline void fill(const_reference value)
		{
			std::fill(this->_data, this->_data + this->_capacity, value);
		}

		inline void swap(dynamic_array<data_type> other) 
		{
			std::swap(this->_data, other->_data);
			std::swap(this->_capacity, other->_capacity);
		}
	};

	template <typename data_type, size_t size>
	class array_vector : protected std::array<data_type, size>
	{
	public:
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