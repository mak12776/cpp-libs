#pragma once

#include <vector>
#include <algorithm>
#include <execution>
#include <random>
#include <variant>
#include <array>

namespace sort
{
	template <typename T>
	using list = std::vector<T>;

	namespace tools
	{
		std::default_random_engine engine;

		template <class T>
		list<T> best_list(size_t size, T initial = std::numeric_limits<T>::min())
		{
			std::vector<T> vec(size);
			std::generate(std::execution::par, vec.begin(), vec.end(), [&] {return initial++; });
			return vec;
		}

		template <class T>
		list<T> worst_list(size_t size, T initial = std::numeric_limits<T>::max())
		{
			std::vector<T> vec(size);
			std::generate(std::execution::par, vec.begin(), vec.end(), [&] {return initial--; });
			return vec;
		}

		template <class T>
		list<T> unique_list(size_t size)
		{
			std::vector<T> vec = best_list<T>(size);
			std::shuffle(vec.begin(), vec.end(), engine);
			return vec;
		}

		template <class T>
		using sub_creator_1 = std::function<list<T>(size_t, T)>;

		template <class T>
		using sub_creator_2 = std::function<list<T>(size_t)>;

		template <class T>
		using list_creator = std::variant<sub_creator_1<T>, sub_creator_2<T>>;

		template <class T, size_t size>
		using list_creator_array = std::array<std::tuple<list_creator<T>, std::string>, size>;
	}

	using data_type = uint32_t;

	tools::list_creator_array<data_type, 3> creator_array = {
			std::make_tuple(tools::best_list<data_type>, std::string("best sort")),
			std::make_tuple(tools::worst_list<data_type>, std::string("worst sort")),
			std::make_tuple(tools::unique_list<data_type>, std::string("unique sort")),
	};


}