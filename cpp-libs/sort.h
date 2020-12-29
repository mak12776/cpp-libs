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
	}

	template <class T>
	using list_creator_with_initial = std::function<list<T>(size_t, T)>;

	template <class T>
	using normal_list_creator = std::function<list<T>(size_t)>;

	template <class T>
	using list_creator = std::variant<list_creator_with_initial<T>, normal_list_creator<T>>;

	template <class T>
	using list_creator_vector = std::vector<std::tuple<list_creator<T>, std::string>>;

	template <typename data_type>
	list_creator_vector<data_type> creator_vector = {
			std::make_tuple(tools::best_list<data_type>, std::string("best sort")),
			std::make_tuple(tools::worst_list<data_type>, std::string("worst sort")),
			std::make_tuple(tools::unique_list<data_type>, std::string("unique sort")),
	};

	template <class T>
	using lt_comp_t = std::function<bool(const T&, const T&)>;

	template <class T>
	using int_comp_t = std::function<bool(const void *, const void *)>;

	template <class T>
	using comp_t = std::variant<lt_comp_t<T>, int_comp_t<T>>;

	template <class T>
	bool lt_comp(const T &first, const T& second) { return first < second; }

	template <class T>
	int int_comp(const void *_first, const void *_second)
	{
		T *first = static_cast<T*>(_first);
		T *second = static_cast<T*>(_second);

		if ((*first) < (*second)) return -1;
		if ((*first) > (*second)) return 1;
		return 0;
	}

	template <class T>
	using lt_sort_function = std::function<void(std::vector<T>&, lt_comp_t<T>)>;

	template <class T>
	using int_sort_function = std::function<void(std::vector<T>&, int_comp_t<T>)>;

	template <class T>
	using sort_function = std::variant<lt_sort_function<T>, int_sort_function<T>>;

	template <typename data_type>
	void std_sort(std::vector<data_type> &vec, lt_comp_t<data_type> comp)
	{
		std::sort(std::execution::seq, vec.begin(), vec.end(), comp);
	}

	template <class T>
	void std_stable_sort(std::vector<T> &vec, lt_comp_t<T> comp)
	{
		std::stable_sort(std::execution::seq, vec.begin(), vec.end(), comp);
	}

	template <class T>
	void std_qsort(std::vector<T> &vec, int_comp_t<T> comp)
	{
		std::qsort(vec.data(), vec.size(), sizeof(T), comp);
	}

	template <class T>
	using sort_function_vector = std::vector<std::tuple<std::string, sort_function<T>, comp_t<T>>>;

	template <class T>
	sort_function_vector<T> sort_vector = {
		std::make_tuple(std::string("std sort"), std_sort<T>, lt_comp<T>),
		std::make_tuple(std::string("std stable sort"), std_stable_sort<T>, lt_comp<T>),
		std::make_tuple(std::string("std stable sort"), std_qsort<T>, int_comp<T>),
	};




	template <typename T>
	void test_sort_functions(list_creator_vector<T> &creator_vector, sort_function_vector<T> &sort_vector)
	{

	}
}