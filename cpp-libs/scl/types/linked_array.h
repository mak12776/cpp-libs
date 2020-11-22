#pragma once

namespace scl
{
	template <typename data_type>
	struct linked_array_node_t
	{
		data_type *pntr;
		linked_array_node_t<data_type> *next;
	};

	template <typename data_type, size_t array_size>
	struct linked_array_t
	{
	};
}
