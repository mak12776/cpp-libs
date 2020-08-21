#pragma once

namespace scl
{
	namespace structs
	{
		template<typename data_type, typename size_type = size_t>
		struct array
		{
			data_type *values;
			size_type size;
		};

		struct buffer : array<void>
		{

		};
	}
}
