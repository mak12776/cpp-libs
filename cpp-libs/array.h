#pragma once

#include <stdlib.h>

namespace scl
{

	template <typename data_type>
	class dynamic_array
	{
	private:
		data_type *_data;
		size_t _capacity;

	public:
		dynamic_array(size_t capacity)
		{
			this->_data = new data_type[capacity];
			this->_capacity = capacity;
		}

		dynamic_array(size_t capacity, data_type value)
		{
			this->_data = new data_type[capacity];
			for (size_t index = 0; index < capacity; index += 1)
			{
				this->_data[index] = value;
			}
			this->_capacity = capacity;
		}

		~dynamic_array()
		{
			delete[] this->_data;
		}

		// member functions

		inline data_type *data() const
		{
			return this->data;
		}

		inline size_t size() const
		{
			return this->_capacity;
		}

		// access functions

		inline data_type &at(size_t index)
		{
			if (index >= this->_capacity)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

			return this->_data[index];
		}

		inline data_type &operator[](size_t index)
		{
			if (index >= this->_capacity)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

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