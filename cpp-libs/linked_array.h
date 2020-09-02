#pragma once

namespace scl
{
	template <typename data_type>
	struct linked_array_node
	{
		data_type *pntr;
		linked_array_node<data_type> *next;

		linked_array_node(size_t size)
		{
			this->pntr = memory::new_array<data_type>(size);
			this->next = nullptr;
		}

		linked_array_node(size_t size, linked_array_node<data_type> *next)
		{
			this->pntr = memory::new_array<data_type>(size);
			this->next = next;
		}
	};

	template <typename data_type>
	struct linked_array
	{
		size_t array_size;
		size_t array_length;
		size_t length;
		linked_array_node<data_type> *first;
		linked_array_node<data_type> *last;

		linked_array(size_t array_size)
		{
			this->array_size = array_size;
			this->array_length = 0;
			this->length = 0;
			this->first = nullptr;
			this->last = nullptr;
		}

		void append(data_type value)
		{
			if (this->first == nullptr)
			{
				this->last = (this->first = new linked_array_node(array_size));
				this->first->pntr[0] = value;
				this->array_size = 1;
			}
			else if (this->array_size == this->array_length)
			{
				this->last = new linked_array_node(array_size, this->last);
				this->last->pntr[0] = value;
				this->array_size = 1;
			}
			else
			{
				this->last->pntr[this->array_size] = value;
				this->array_size += 1;
			}
			this->length += 1;
		}
	};

}
