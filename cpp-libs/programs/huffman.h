#pragma once

#include "../scl/all.h"

namespace bith
{
	using namespace scl;

	template <typename data_type>
	struct data_count_t
	{
		data_type data;
		size_t count;
	};

	template <typename data_type>
	struct remaining_t
	{
		data_type value;
	};

	struct segmented_buffer_t
	{

	};
}