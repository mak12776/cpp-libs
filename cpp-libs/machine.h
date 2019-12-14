#pragma once

#include <cstdint>

#include "array.h"
#include "buffer.h"

namespace scl
{
	typedef struct
	{
		fixed_array_t *buffers;
		fixed_array_t *dp;

		size_t ip;
		size_t bip;
	} machine_t;

	namespace machine
	{
		typedef uint8_t inst_t;

		const inst_t INST_NOOP = 0x00;

		static inline void run(machine_t *machine)
		{

		}
	}
}
