#pragma once

namespace scl
{
	template <typename byte_type>
	struct base_buffer_t
	{
		byte_type *pntr;
		size_t size;
	};

	typedef base_buffer_t<void> vbuffer_t;
	typedef base_buffer_t<ubyte> ubuffer_t;
	typedef base_buffer_t<byte> buffer_t;
}