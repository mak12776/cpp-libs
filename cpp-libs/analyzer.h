#pragma once

#include <cstdint>

#include "buffer.h"

namespace scl
{
	namespace analyzer
	{
		typedef struct
		{
			buffer::buffer_t inst_buffer;
			
		} analyzer_t;

		static inline void execute(analyzer_t analyzer)
		{

		}
	}
}
