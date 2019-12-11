#pragma once

#include <cstdint>

#include "error.h"

#include "types.h"
#include "io.h"

namespace scl
{
	namespace buffer
	{
		typedef struct
		{
			byte *pntr;
			size_t size;
		} buffer_t;

		static inline void initialize(buffer_t *buffer, size_t size)
		{
			buffer->pntr = new byte[size];
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

			result->pntr = new byte[size];
			result->size = size;

			return result;
		}

		static inline void delete_buffer(buffer_t *buffer)
		{
			delete[] buffer->pntr;
			delete buffer;
		}

		static inline size_t count_lines(buffer_t *buffer)
		{
			size_t index;
			size_t total;

			if (buffer->size == 0)
			{
				error::num = error::ERROR_BAD_ARGUMENT;
				error::set_info_function_name("count_lines");
				error::set_info_argument_name("buffer->size");
			}

			index = 0;
			total = 0;

#define CH (buffer->pntr[index])
#define END (index == buffer->size)
#define INC_INDEX_CHECK_END {index += 1; if (END) return total;}

		loop:
			if (CH == '\n')
			{
				total += 1;

				goto next;
			}

		check_cr:
			if (CH == '\r')
			{
				total += 1;

				INC_INDEX_CHECK_END;

				if (CH == '\n')
					goto next;

				goto check_cr;
			}

			index += 1;
			if (END)
			{
				total += 1;
				return total;
			}

			goto loop;

		next:
			INC_INDEX_CHECK_END;

			goto loop;

#undef CH
#undef END
#undef INC_INDEX_CHECK_END
		}
	}

	namespace view
	{
		typedef struct
		{
			size_t start;
			size_t end;
		} view_t;
	}

	namespace packed_view
	{
		typedef struct
		{
			byte *pntr;
			view::view_t view;
		} packed_view_t;
	}

	namespace buffer_views
	{
		typedef struct
		{
			buffer::buffer_t buffer;
			view::view_t *views;
			size_t total;
		} buffer_views_t;
	}
}

