
#include "scl/all.h"

namespace sort
{
	template <typename data_t>
	using array_t = scl::dynamic_array_t<data_t>;

	typedef std::mt19937_64 default_random_engine;

	namespace tools
	{
		template <typename data_t>
		void print_array(array_t<data_t> list)
		{
			std::cout << "[" << list.size() << "]: {";
			if (!list.empty())
			{
				std::cout << list[0];
				for (size_t index = 1; index < list.size(); index += 1)
					std::cout << ", " << list[index];
			}
			std::cout << "}" << std::endl;
		}

		template <typename data_type>
		array_t<data_type> copy_array(array_t<data_type> array)
		{
			array_t<data_type> result;
			result.allocate(result.size()); ERR_CHECK_RETURN_VALUE(result);
			std::copy(array.begin(), array.end(), result.begin());
			return result;
		}


		template <typename data_type>
		array_t<data_type> best_array(size_t size)
		{
			array_t<data_type> result;

			result.allocate(size); ERR_CHECK_RETURN_VALUE(result);
			data_type step = 0;
			for (size_t index = 0; index < result.size(); index++)
				result[index] = step++;

			return result;
		}

		template <typename data_type>
		array_t<data_type> unique_array(size_t size)
		{
			default_random_engine engine;
			array_t<data_type> result;
			
			result = best_array<data_type>(size); ERR_CHECK_RETURN_VALUE(result);
			std::shuffle(result.data(), result.data() + result.size(), engine);
			return result;
		}

		template <typename data_type>
		array_t<data_type> worst_array(size_t size)
		{
			array_t<data_type> result;
			data_type step = (data_type)size;

			result.allocate(size); ERR_CHECK_RETURN_VALUE(result);
			for (size_t index = 0; index < result.size(); index++)
				result[index] = --step;

			return result;
		}

		template <typename data_type>
		array_t<data_type> random_array(size_t size)
		{
			default_random_engine engine;
			std::uniform_int dist(std::numeric_limits<data_type>::min(), std::numeric_limits<data_type>::max());
			array_t<data_type> result;

			result.allocate(size); ERR_CHECK_RETURN_VALUE(result);
			for (size_t index = 0; index < result.size(); index += 1)
				result[index] = dist(engine);

			return result;
		}


		template <typename data_type>
		int data_comp(const void *_first, const void *_second)
		{
			const data_type *first = static_cast<const data_type*>(_first);
			const data_type *second = static_cast<const data_type*>(_second);

			if ((*first) < (*second)) return -1;
			if ((*first) > (*second)) return 1;
			return 0;
		}
	}

	template <typename data_t>
	using comp_t = int(*)(const void *, const void *);

	template <typename data_t>
	using sort_func_t = void(*)(array_t<data_t>, comp_t<data_t>);

	template <typename data_t>
	void insertion_sort(array_t<data_t> list, comp_t<data_t> comp)
	{
		size_t index_i = 1;
		while (index_i < list.size())
		{
			size_t index_j = index_i;
			while ((index_j > 0) && (comp(list[index_j - 1], list[index_j]) == 1))
			{
				std::swap(list[index_j - 1], list[index_j]);
				index_j -= 1;
			}
			index_i += 1;
		}
	}

	template <typename data_t>
	void std_sort(array_t<data_t> array, comp_t<data_t> comp)
	{
		std::sort(std::execution::seq, array.begin(), array.end(), comp);
	}

	template <typename data_t>
	void std_stable_sort(array_t<data_t> array, comp_t<data_t> comp)
	{
		std::stable_sort(std::execution::seq, array.begin(), array.end(), comp);
	}
	
	template <typename data_t>
	void std_qsort(array_t<data_t> array, comp_t<data_t> comp)
	{
		std::qsort(array.data(), array.size(), sizeof(array.value_type), comp);
	}

	template <typename data_t>
	struct func_list_t
	{
		struct
		{
			const char *name;
			sort_func_t<data_t> func;
		} *list;
		comp_t<data_t> comp;
	};

	typedef uint32_t DataType;

	comp_t<DataType> simple_comp = tools::data_comp<DataType>;

	namespace linked_tree_sort
	{
		enum flag_t : uint8_t
		{
			AVAIBLE = 0x80,
			HAS_LEFT = 0x40,
			HAS_RIGHT = 0x20,
		};

		struct chain_node
		{
			size_t left;
			size_t right;
		};

		struct list_tree
		{
			flag_t *flag_pntr;
			chain_node *chain_pntr;

			void allocate(size_t size)
			{
				chain_pntr = scl::mem::safe_malloc_array<chain_node>(size); 
				ERR_CHECK_RETURN;


			}
		};
	}
}