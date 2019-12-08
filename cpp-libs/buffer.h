#pragma once

#include <cstdint>
#include "types.h"
#include "io.h"

using namespace scl;

namespace scl
{
	namespace buffer
	{
		typedef struct
		{
			char_t *pntr;
			size_t size;
		} buffer_t;

		static inline void initialize(buffer_t *buffer, size_t size)
		{
			buffer->pntr = new char_t[size];
			buffer->size = size;
		}

		static inline void finalize(buffer_t *buffer)
		{
			delete[] buffer->pntr;
			buffer->size = 0;
		}

		static inline buffer_t *new_buffer(size_t size)
		{
			buffer_t *result = new buffer_t;

			result->pntr = new char_t[size];
			result->size = size;

			return result;
		}

		static inline void delete_buffer(buffer_t *buffer)
		{
			delete[] buffer->pntr;
			delete buffer;
		}

		typedef struct
		{
			size_t start;
			size_t end;
		} view_t;

		typedef struct
		{
			char_t *pntr;
			view_t view;
		} packed_view_t;

	}
}

