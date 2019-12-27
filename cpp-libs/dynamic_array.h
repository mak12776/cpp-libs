#pragma once

#include "memory.h"

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
		dynamic_array()
		{
			this->_data = nullptr;
			this->_capacity = 0;
		}

		dynamic_array(size_type capacity)
		{
			this->_data = (capacity != 0) ? new data_type[capacity] : nullptr;
			this->_capacity = capacity;
		}

		inline void dump()
		{
			std::cout << "size: " << this->_capacity << std::endl;
			for (size_type index = 0; index < this->_capacity; index += 1)
				std::cout << std::hex << this->_data[index] << std::dec << " ";
			std::cout << std::endl;
		}

		dynamic_array(size_type capacity, const_reference value)
		{
			this->_capacity = capacity;
			this->_data = (capacity != nullptr) ? memory::new_array<value_type>(capacity, value) : nullptr;
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

		inline const_pointer data() const
		{
			return this->_data;
		}

		inline const_reference first() const
		{
			if (this->_capacity == 0)
				throw new std::out_of_range("array is empty");
				
			return this->_data[0];
		}

		inline const_reference last() const
		{
			if (this->_capacity == 0)
				throw new std::out_of_range("array is empty");

			return this->_data[this->_capacity - 1];
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
}
