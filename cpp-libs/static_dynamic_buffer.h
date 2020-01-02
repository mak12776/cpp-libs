#pragma once

#include "static_dyanamic_array.h"

namespace scl
{
	class static_dyanmic_buffer : public static_dynamic_array<uint8_t>
	{
	public:
		static_dyanmic_buffer(size_type capacity)
			: static_dynamic_array(capacity)
		{ }

		static_dyanmic_buffer(pointer data, size_type capacity)
			: static_dynamic_array(data, capacity)
		{ }

		static_dyanmic_buffer(size_type capacity, const_reference value)
			: static_dynamic_array(capacity, value)
		{ }

	};
}