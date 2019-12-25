#pragma once

#include <stdlib.h>

namespace scl
{
	template <typename data_type>
	struct fixed_array
	{
		data_type *pntr;
		size_t size;
	};

	template <typename type, size_t size>
	class fixed_vector : protected std::array<type, size>
	{
	protected:
		size_t _lenght;

	public:
		fixed_vector()
		{
			this->_lenght = 0;
		}

		fixed_vector(size_t lenght, type value)
		{
			if (lenght >= this->size())
				throw new std::out_of_range("lenght is greater than array size");

			for (size_t index = 0; index < lenght; index += 1)
			{
				this->array::_Elems[index] = value;
			}
			this->_lenght = lenght;
		}

		inline constexpr size_t size() const
		{
			return this->array::size();
		}

		inline size_t lenght() const
		{
			return this->_lenght;
		}
		
		inline type at(size_t index)
		{
			if (index >= this->_lenght)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

			return this->_Elems[index];
		}

		inline type &operator[](size_t index)
		{
			if (index >= this->_lenght)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

			return this->_Elems[index];
		}
	};
}