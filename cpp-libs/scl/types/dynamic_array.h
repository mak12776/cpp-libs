#pragma once

namespace scl
{
	template <typename byte_type>
	struct dynamic_array_t
	{
		byte_type *pntr;
		size_t size;

		// malloc fread, fwrite
		
		// TODO: add malloc fread, fwrite

		// fread, fwrite

		inline size_t fread(FILE *file)
		{
			size_t read_number = io::fread_array<byte_type>(pntr, size, file);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return read_number;
		}

		inline size_t fwrite(FILE *file)
		{
			size_t write_number = io::fwrite_array<byte_type>(pntr, size, file);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return write_number;
		}
		
		// fopen fread, fwrite

		inline size_t fopen_fread(const char *name)
		{
			size_t read_number = io::fopen_fread(name, pntr, size);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return read_number;
		}

		inline size_t fopen_fwrite(const char *name)
		{
			size_t write_number = io::fopen_fwrite(name, pntr, size);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return write_number;
		}
	};
}