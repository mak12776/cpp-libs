#pragma once

#include <cstdint>

#include "array.h"
#include "buffer.h"

namespace scl
{
	namespace machines
	{
		typedef uint8_t inst_t;

		const inst_t INST_NOOP = 0x00;

		template <typename data_type, size_t pointers_number, size_t buffer_number>
		class data_machine
		{
		private:
			fixed_vector<data_type *, buffer_number> data_arrays;
			fixed_vector<size_t, pointers_number> dp;

			size_t ip;
			size_t bip;

		public:

		};
	}
}
