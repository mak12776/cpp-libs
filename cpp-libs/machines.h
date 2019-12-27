#pragma once

#include <cstdint>
#include <vector>

#include "array.h"
#include "vector.h"
#include "buffer.h"

namespace scl
{
	namespace data_machine
	{
		typedef uint8_t inst_t;

		const inst_t INST_NOOP = 0x00;

		template <typename data_type, size_t pointers_number, size_t buffer_number>
		class machine
		{
		private:
			array_vector<fixed_vector<data_type>, buffer_number> data_arrays;
			array_vector<size_t, pointers_number> data_pointers;

			size_t ip;
			size_t bip;

		public:

		};
	}

	namespace buffer_machine
	{
		constexpr uint8_t inst_size = 1;

		typedef uint8_t inst_t;

		const inst_t INST_NOOP = 0x00;

		class machine
		{
		private:
			dynamic_array<dynamic_array<uint8_t> *> buffers;
			dynamic_array<size_t> pointers;

			size_t ip;
			size_t bip;

		public:
			machine(size_t buffers_number, size_t pointers_number, size_t base_inst_pointer, size_t inst_pointer)
				: buffers(buffers_number), pointers(pointers_number), bip(base_inst_pointer), ip(inst_pointer)
			{ }

			inline void execute()
			{
				if (bip >= buffers.size())
				{

				}

				if (buffers[bip] == nullptr)
				{

				}

				if (ip >= buffers[bip]->size())
				{

				}
			}
		};
	}
}
