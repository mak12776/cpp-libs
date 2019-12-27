#pragma once

#include <string>
#include <stdexcept>

namespace scl
{
	namespace safe
	{
		constexpr bool check_index_out_of_range = true;

		template <typename type>
		static inline void throw_if_out_of_range(type value, type max, const std::string name = "index", std::string message = "")
		{
			if (value >= max)
			{
				if (message.empty())
					message = name + std::string("is out of range: ") + std::to_string(value);
				else
					message += ": " + std::to_string(value);

				throw new std::out_of_range(message);
			}
		}
	}
}
