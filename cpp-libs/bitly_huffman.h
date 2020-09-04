#pragma once

#include "string.h"

namespace bh
{
	scl::c_string_t archive_ext(".64bh");
	const char magic[] = { '\x64' };

	void compress(const char *input_name_pntr, const char *output_name_pntr)
	{
		scl::c_string_t file_name(input_name_pntr);
		scl::c_string_t archive_name(output_name_pntr);


	}

}