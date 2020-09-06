#pragma once

#include "err.h"

namespace bh
{
	using namespace scl;

	scl::c_string_t archive_ext(".64bh");
	const char magic[] = { '\x64' };

	void compress(const char *file_name_pntr)
	{
		scl::c_string_t file_name(file_name_pntr);
		scl::m_string_t archive_name;

		printf("In The Name Of God.\n");

		io::set_default_width(20);
		io::print_line();

		// malloc for archive_name
		archive_name.malloc_cat({ file_name, archive_ext });
		if (err::check())
		{
			printf("error: can't malloc memory for archive file name.");
			return;
		}
		clean_up::add_free(archive_name.pntr);

		ubuffer_t buffer;
		size_t file_bits;
		FILE *archive_file;

		io::read_file_name(file_name.pntr, (void **)&buffer.pntr, &buffer.size);
		if (err::check())
		{
			printf("error: can't read '%s': %s\n", file_name.pntr, 
				err::get_string());
			printf("errno: %s\n", strerror(errno));

			clean_up::finish();
			return;
		}
		clean_up::add_free(buffer.pntr);

		math::safe_mul(buffer.size, (size_t)8, file_bits);
		if (err::check())
		{
			printf("error: can't multiply bytes number by 8.\n");

			clean_up::finish();
			return;
		}

		archive_file = io::safe_fopen(archive_name.pntr, "wb");
		if (err::check())
		{
			printf("error: can't fopen '%s' for writing.\n", archive_name.pntr);
			printf("errno: %s\n", strerror(errno));

			clean_up::finish();
			return;
		}
		clean_up::add_fclose(archive_file);

		printf("file name: %s\n", file_name.pntr);
		printf("archive name: %s\n", archive_name.pntr);
		printf("file size: (%zu bytes) (%zu bits)\n", buffer.size, file_bits);

		io::print_line();

		clean_up::finish();
	}

}