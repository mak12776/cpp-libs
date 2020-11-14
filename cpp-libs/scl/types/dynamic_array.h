#pragma once

namespace scl
{
	template <typename byte_type>
	struct dynamic_array_t
	{
		byte_type *pntr;
		size_t size;

		inline void fread(FILE *file)
		{
			io::fread_array<byte_type>(pntr, size, file);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}

		inline void fopen_fread(const char *name)
		{
			io::fopen_fread(name, pntr, size);

		}

		inline void fwrite(FILE *file)
		{
			io::fwrite_array<byte_type>(pntr, size, file);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}

		inline void fopen_fwrite(const char *name)
		{

		}
	};

#ifdef SCL_EXPERIMENTAL
	namespace io
	{
		// fread, fwrite malloc with base_buffer_t

		template <typename byte_t>
		static inline void fread_malloc(FILE *file, dynamic_array_t<byte_t> &buffer)
		{
			fread_malloc(file, (void **)&buffer.pntr, buffer.size);
		}

		template <typename byte_t>
		static inline void fopen_fread_malloc(const char *name, dynamic_array_t<byte_t> &buffer)
		{
			fopen_fread_malloc(name, (void **)&buffer.pntr, &buffer.size);
		}

		template <typename byte_t>
		static inline void fopen_fwrite(const char *file, dynamic_array_t<byte_t> &buffer)
		{
			fopen_fwrite(file, (void *)buffer.pntr, buffer.size);
		};
	}
#endif // SCL_EXPERIMENTAL
}