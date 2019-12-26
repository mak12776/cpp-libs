#pragma once

#include <stdexcept>
#include <string>

namespace scl
{
	template <typename data_type>
	class fixed_vector
	{
	private:
		data_type *_data;
		size_t _capacity;
		size_t _size;

	public:
		fixed_vector(size_t capacity)
		{
			this->_data = new data_type[capacity];
			this->_capacity = capacity;
			this->_size = 0;
		}

		fixed_vector(size_t capacity, size_t size, data_type value)
		{
			if (size >= capacity)
				throw new std::out_of_range("size is greater than capacity: " + std::to_string(size));

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


	template <typename data_type>
	class linked_vector_list
	{

	};
}