#pragma once

#include <cstdint>

#include "buffer.h"

namespace scl
{
	namespace machine
	{
		typedef struct
		{
			buffer::buffer_t inst_buffer;
			
		} machine_t;

		typedef uint8_t inst_t;

		static inline void execute(machine_t &machine)
		{

		}


	}
}
