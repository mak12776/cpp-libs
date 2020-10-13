#pragma once

#include <string>
#include <stdexcept>

namespace scl
{
	namespace safe
	{
		constexpr bool error_index_out_of_range = true;

		template <typename type>
		static inline void check_index_out_of_range(type value, type max, const std::string name = "index", std::string message = "")
		{
			if (value >= max)
			{
				if (message.empty())
					message = name + std::string("test out of range: ") + std::to_string(value);
				else
					message += ": " + std::to_string(value);

				throw new std::out_of_range(message);
			}
		}

		constexpr bool error_zero_size = true;

		template <typename type>
		static inline void check_zero_size(type value, const std::string name = "size", std::string message = "")
		{
			if (value == 0)
			{
				if (message.empty())
					message = name + std::string("test out of range: ") + std::to_string(value);

				throw new std::out_of_range(message);
			}
		}
	}
}
