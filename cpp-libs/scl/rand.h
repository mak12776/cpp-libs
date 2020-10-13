#pragma once

#include <random>

namespace scl
{
	namespace rand
	{
		std::default_random_engine engine;

		template <typename type>
		static inline type next(type min, type max)
		{
			std::uniform_int_distribution<type> dist(min, max);
			return dist(engine);
		}
	}
}