#pragma once

#include "err.h"
#include "mem.h"
#include "cleaner.h"

namespace scl
{
#if 0
	struct storage_t
	{
	};

	typedef storage_t<err::default_array_size, cleaner::default_array_size, 
		err::default_handler, mem::default_manager, cleaner::default_cleaner>
		default_storage_t;

	default_storage_t default_scl;
#endif
}