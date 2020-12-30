#pragma once

#include <vector>
#include <algorithm>
#include <execution>
#include <random>
#include <variant>
#include <array>
#include <tuple>

namespace sort
{
	template <typename T>
	using list = std::vector<T>;

	namespace tools
	{
		// list creators

		std::default_random_engine engine;

		template <class data_type>
		list<data_type> best_list(size_t size)
		{
			data_type value = std::numeric_limits<data_type>::min();
			std::vector<data_type> vec(size);
			std::generate(std::execution::par, vec.begin(), vec.end(), [&] {return value++; });
			return vec;
		}

		template <class data_type>
		list<data_type> worst_list(size_t size)
		{
			data_type value = std::numeric_limits<data_type>::max();
			std::vector<data_type> vec(size);
			std::generate(std::execution::par, vec.begin(), vec.end(), [&] {return value--; });
			return vec;
		}

		template <class data_type>
		list<data_type> unique_list(size_t size)
		{
			std::vector<data_type> vec = best_list<data_type>(size);
			std::shuffle(vec.begin(), vec.end(), engine);
			return vec;
		}

		template <class data_type>
		using list_creator = std::function<list<data_type>(size_t)>;

		template <class data_type>
		using list_creator_vector = std::vector<std::tuple<std::string, list_creator<data_type>>>;

		template <class data_type>
		list_creator_vector<data_type> creator_vector = {
			std::make_tuple(std::string("best sort"), best_list<data_type>),
			std::make_tuple(std::string("worst sort"), worst_list<data_type>),
			std::make_tuple(std::string("unique sort"), unique_list<data_type>),
		};

		// compare functions

		template <class data_type>
		using lt_comp_t = std::function<bool(const data_type&, const data_type&)>;
		using int_comp_t = int(*)(const void *, const void *);

		template <class data_type>
		using comp_t = std::variant<lt_comp_t<data_type>, int_comp_t>;

		static size_t compares_number = 0;
		void reset_compares_number() { compares_number = 0; }

		template <class data_type>
		bool lt_comp(const data_type &first, const data_type& second) 
		{ 
			compares_number += 1;
			return first < second;
		}

		template <class data_type>
		int int_comp(const void *_first, const void *_second)
		{
			const data_type *first = static_cast<const data_type*>(_first);
			const data_type *second = static_cast<const data_type*>(_second);

			compares_number += 1;
			if ((*first) < (*second)) return -1;
			if ((*first) > (*second)) return 1;
			return 0;
		}

		// sort functions

		template <class data_type>
		using lt_sort_function = std::function<void(std::vector<data_type>&, lt_comp_t<data_type>)>;

		template <class data_type>
		using int_sort_function = std::function<void(std::vector<data_type>&, int_comp_t)>;

		template <class data_type>
		using sort_function = std::variant<lt_sort_function<data_type>, int_sort_function<data_type>>;

		template <class data_type>
		using sort_function_vector = std::vector<std::tuple<std::string, sort_function<data_type>>>;

	}

	template <typename data_type>
	void std_sort(std::vector<data_type> &vec, tools::lt_comp_t<data_type> comp)
	{
		std::sort(std::execution::seq, vec.begin(), vec.end(), comp);
	}

	template <class data_type>
	void std_stable_sort(std::vector<data_type> &vec, tools::lt_comp_t<data_type> comp)
	{
		std::stable_sort(std::execution::seq, vec.begin(), vec.end(), comp);
	}

	template <class data_type>
	void std_qsort(std::vector<data_type> &vec, tools::int_comp_t comp)
	{
		std::qsort(vec.data(), vec.size(), sizeof(data_type), comp);
	}

	template <class data_type>
	tools::sort_function_vector<data_type> sort_vector = {
		std::make_tuple(std::string("std sort"), std_sort<data_type>),
		std::make_tuple(std::string("std stable sort"), std_stable_sort<data_type>),
		std::make_tuple(std::string("std qsort"), std_qsort<data_type>),
	};



	template <class data_type>
	void test_functions(tools::list_creator_vector<data_type> &creator_vector, tools::sort_function_vector<data_type> &sort_vector, size_t size)
	{
		std::vector<std::vector<data_type>> vectors(creator_vector.size());

		// creating vectors
		for (size_t index = 0; index < vectors.size(); index += 1)
		{
			tools::list_creator<data_type> creator = std::get<tools::list_creator<data_type>>(creator_vector[index]);
			vectors[index] = creator(size);
		}

		// testings sort functions
	}

	template <typename data_type>
	void test_default_functions(size_t size)
	{
		return test_functions(tools::creator_vector<data_type>, sort_vector<data_type>, size);
	}
}