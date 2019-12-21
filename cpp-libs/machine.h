#pragma once

#include <cstdint>

#include "array.h"
#include "buffer.h"

namespace scl
{
	namespace machine
	{
		typedef uint8_t inst_t;

		const inst_t INST_NOOP = 0x00;

		class machine
		{
			fixed_array *buffers;
			fixed_array *dp;

			size_t ip;
			size_t bip;
		};
	}
}
