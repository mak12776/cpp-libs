#pragma once

#include <stdlib.h>

namespace scl
{
	template <typename data_type, size_t size>
	class array_vector : protected std::array<data_type, size>
	{
	protected:
		size_t _lenght;

	public:
		array_vector()
		{
			this->_lenght = 0;
		}

		array_vector(size_t lenght, data_type value)
		{
			if (lenght >= this->size())
				throw new std::out_of_range("lenght is greater than array size: " + std::to_string(lenght));

			for (size_t index = 0; index < lenght; index += 1)
			{
				this->array::_Elems[index] = value;
			}
			this->_lenght = lenght;
		}

		inline data_type *data() const
		{
			return this->_Elems;
		}

		inline constexpr size_t size() const
		{
			return this->array::size();
		}

		inline size_t lenght() const
		{
			return this->_lenght;
		}
		
		inline data_type at(size_t index)
		{
			if (index >= this->_lenght)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

			return this->_Elems[index];
		}

		inline data_type &operator[](size_t index)
		{
			if (index >= this->_lenght)
				throw new std::out_of_range("index is out of range: " + std::to_string(index));

			return this->_Elems[index];
		}
	};
}