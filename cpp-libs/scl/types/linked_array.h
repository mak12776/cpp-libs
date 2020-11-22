#pragma once

namespace scl
{
	template <typename data_type, size_t array_size>
	struct linked_array_node_t
	{
		data_type pntr[array_size];
		linked_array_node_t<data_type, array_size> *next;
	};

	template <typename data_type, size_t array_size>
	struct linked_array_t
	{
		typedef data_type& reference;
		typedef const data_type& const_reference;

		size_t len;
		linked_array_node_t<data_type, array_size> *first;
		linked_array_node_t<data_type, array_size> *last;

		linked_array_t()
			: len(0), first(nullptr), last(nullptr) { }

		inline bool append(const_reference value)
		{
			if (len == 0)
			{
				first = mem::malloc(sizeof linked_array_node_t<data_type, array_size>);
				if (first == nullptr)
					return true;

				last = first;
			}
			else if (len % array_size == 0)
			{
				last->next = mem::malloc(sizeof linked_array_node_t<data_type, array_size>);
				if (last->next == nullptr)
					return true;

				last = last->next;
			}

			last[len % array_size] = value;
			len += 1;
		}
	};
}
