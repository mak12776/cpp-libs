#pragma once

#include <random>

namespace scl
{
	namespace rand
	{
		std::default_random_engine engine;

		template <typename T>
		static inline T next_int(T min, T max)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(engine);
		}
	}
}