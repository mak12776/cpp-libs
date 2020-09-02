#pragma once

namespace bh
{
	c_string_t archive_ext(".64bh");
	const char magic[] = { '\x64' };

	void compress(const char *file_name_pntr)
	{
		c_string_t file_name(file_name_pntr);

		io::read_file_name(file_name.pntr, );
	}

}