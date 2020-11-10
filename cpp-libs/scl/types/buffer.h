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

	namespace io
	{
		// fread, fwrite malloc with base_buffer_t

		template <typename byte_t>
		static inline void fread_malloc(FILE *file, base_buffer_t<byte_t> &buffer)
		{
			fread_malloc(file, (void **)&buffer.pntr, buffer.size);
		}

		template <typename byte_t>
		static inline void fopen_fread_malloc(const char *name, base_buffer_t<byte_t> &buffer)
		{
			fopen_fread_malloc(name, (void **)&buffer.pntr, buffer.size);
		}

		template <typename byte_t>
		static inline void fopen_fwrite(const char *file, base_buffer_t<byte_t> &buffer)
		{
			fopen_fwrite(file, (void *)buffer.pntr, buffer.size);
		};
	}
}