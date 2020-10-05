#pragma once

#include "err.h"
#include "mem.h"
#include "cleaner.h"

namespace scl
{
	template <
		size_t handler_size,
		size_t cleaner_size,
		err::handler_t<handler_size> &handler,
		mem::manager_t<handler_size, handler> &_manager,
		cleaner::cleaner_t<cleaner_size> &_cleaner>
	struct storage_t
	{
		err::handler_t<handler_size> &err = handler;
		mem::manager_t<handler_size, handler> mem = _manager;
		cleaner::cleaner_t<cleaner_size> &cleaner = _cleaner;
	};

	typedef storage_t<err::default_array_size, cleaner::default_array_size, 
		err::default_handler, mem::default_manager, cleaner::default_cleaner>
		default_storage_t;

	default_storage_t default_scl;
}