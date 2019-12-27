#pragma once

#include <string>
#include <stdexcept>

namespace scl
{
	namespace safe
	{
		constexpr bool check_index_out_of_range = true;

		template <typename type>
		static inline void is_index_out_of_range(type index, type max, const std::string name = "index", const std::string message = "")
		{
			if (message.empty())
				message = name + std::string("is out of range: ");

			if (index >= max)
				throw new std::out_of_range(message);
		}
	}
}
