
#include "scl/all.h"

namespace sort
{
	template <typename data_type>
	void insertion_sort(data_type *pntr, size_t count, int(comp)(const data_type &, const data_type &))
	{
		size_t index_i = 1;
		while (index_i < count)
		{
			size_t index_j = index_i;
			while ((index_j > 0) && (comp(pntr[index_j - 1], pntr[index_j]) == 1))
				std::swap(pntr[index_j - 1], pntr[index_j]);
			index_i += 1;
		}
	}
}