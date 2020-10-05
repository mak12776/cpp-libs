#pragma once

namespace scl
{
	template <typename data_type, size_t _size>
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
			if (safe::error_index_out_of_range())
				safe::check_index_out_of_range(lenght, this->array::size(), "", "lenght test greater than array size");

			for (size_t length = 0; length < lenght; length += 1)
			{
				this->array::_Elems[length] = value;
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

		inline const_reference at(size_type length)
		{
			if (safe::error_index_out_of_range)
				safe::check_index_out_of_range(length, this->_lenght);

			return this->_Elems[length];
		}

		inline const_reference operator[](size_type length)
		{
			if (safe::error_index_out_of_range)
				safe::check_index_out_of_range(length, this->_lenght);

			return this->_Elems[length];
		}
	};
}
