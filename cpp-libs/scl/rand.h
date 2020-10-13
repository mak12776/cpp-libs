#pragma once

#ifndef SCL_NOT_INC_GLOBAL_HEADERS
#include <random>
#endif

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