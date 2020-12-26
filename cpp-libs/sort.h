
#include "scl/all.h"

namespace sort
{
	template <typename data_t>
	void print_vector(std::vector<data_t> vec)
	{
		for (size_t index = 0; index < vec.size(); index += 1)
			std::cout << vec[index] << ", ";
		std::cout << std::endl;
	}

	template <typename data_t>
	void print_array(data_t *pntr, size_t size)
	{
		for (size_t index = 0; index < size; index += 1)
			std::cout << pntr[index] << ", ";
		std::cout << std::endl;
	}

	template <typename data_t>
	using comp_t = int(const data_t &, const data_t &);

	// sort functions
	template <typename data_t>
	using sort_func_t = void(data_t *, size_t, comp_t<data_t>*);

	template <typename data_t>
	void insertion_sort(data_t *pntr, size_t count, comp_t<data_t> comp)
	{
		size_t index_i = 1;
		while (index_i < count)
		{
			size_t index_j = index_i;
			while ((index_j > 0) && (comp(pntr[index_j - 1], pntr[index_j]) == 1))
			{
				std::swap(pntr[index_j - 1], pntr[index_j]);
				index_j -= 1;
			}
			index_i += 1;
		}
	}

	template <typename data_t>
	void vector(sort_func_t<data_t> func, std::vector<data_t> &vec, comp_t<data_t> comp)
	{
		func(vec.data(), vec.size(), comp);
	}

	// vector generations

	template <typename data_t>
	std::vector<data_t> unique_vector(size_t size)
	{
		std::default_random_engine engine;

		std::vector<data_t> result(size);
		data_t step = 0;
		for (size_t index = 0; index < result.capacity(); index += 1) 
			result[index] = step++;
		std::shuffle(result.begin(), result.end(), engine);
		return result;
	}

	template <typename data_t>
	std::vector<data_t> best_vector(size_t size)
	{
		std::vector<data_t> result(size);
		data_t step = 0;
		for (size_t index = 0; index < result.capacity(); index += 1)
			result[index] = step++;
		return result;
	}

	template <typename data_t>
	std::vector<data_t> worst_vector(size_t size)
	{
		std::vector<data_t> result(size);
		data_t step = (data_t)(size - 1);
		for (size_t index = 0; index < result.capacity(); index += 1)
			result[index] = step--;
		return result;
	}

	template <typename data_t>
	std::vector<data_t> random_vector(size_t size)
	{
		std::default_random_engine engine;
		std::uniform_int dist;

		std::vector<data_t> result(size);
		for (size_t index = 0; index < result.capacity(); index += 1)
		result[index] = dist(engine);
		return result;
	}
}