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
		size_t _lenght;
		size_t _size;

	public:
		fixed_vector(size_t size)
		{
			this->_data = new data_type[size];
			this->_size = size;
			this->_lenght = 0;
		}

		fixed_vector(size_t size, size_t lenght, data_type value)
		{
			if (lenght >= size)
				throw new std::out_of_range("lenght is out of range: " + std::to_string(lenght));

			this->_data = new data_type[size];
			for (size_t index = 0; index < lenght; index += 1)
			{
				this->_data[index] = value;
			}
			this->_lenght = lenght;
		}


	}
}