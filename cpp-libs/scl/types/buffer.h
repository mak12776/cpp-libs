#pragma once

namespace scl
{
	template <typename byte_type>
	struct base_buffer_t
	{
		byte_type *pntr;
		size_t size;

		inline size_t fread(FILE *file)
		{
			return io::fread_array<byte_type>(pntr, size, file);
		}
	};

	typedef base_buffer_t<void> vbuffer_t;
	typedef base_buffer_t<ubyte> ubuffer_t;
	typedef base_buffer_t<byte> buffer_t;

	// fread, fwrite all with base_buffer_t

	/*template <typename byte_t>
	static inline void fread_all(FILE *file, base_buffer_t<byte_t> &buffer)
	{
		fread_all(file, (void **)&buffer.pntr, buffer.size);
	}

	template <typename byte_t>
	static inline void fopen_fread_all(const char *file, base_buffer_t<byte_t> &buffer)
	{
		fopen_fread_all(file, (void **)&buffer.pntr, buffer.size);
	}

	template <typename byte_t>
	static inline void fopen_fwrite_all(const char *file, base_buffer_t<byte_t> &buffer)
	{
		fopen_fwrite_all(file, (void *)buffer.pntr, buffer.size);
	};*/
}