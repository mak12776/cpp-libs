#pragma once

namespace scl
{
	typedef void (&dynamic_array_size_manager_t)(size_t &size);

	template <typename data_type>
	struct dynamic_array_t
	{
		typedef data_type &reference;
		typedef const data_type &const_reference;

		data_type *pntr;
		size_t size;

		dynamic_array_t()
			: pntr(nullptr), size(0) {}
		
		// malloc fread, fwrite

		inline void allocate(size_t size)
		{
			pntr = mem::safe_malloc_array<data_type>(size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;
			this->size = size;
		}

		inline void reallocate(size_t size)
		{
			pntr = mem::safe_realloc_array<data_type>(size);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return;
			this->size = size;
		}

		inline void deallocate()
		{
			mem::free(pntr);
			this->size = 0;
		}

		// fread, fwrite

		inline size_t fread(FILE *file)
		{
			size_t read_number = io::fread_array<data_type>(pntr, size, file);
			err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return read_number;
		}

		inline size_t fwrite(FILE *file)
		{
			size_t write_number = io::fwrite_array<data_type>(pntr, size, file);
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