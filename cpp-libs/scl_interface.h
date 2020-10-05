#pragma once

#include "err.h"
#include "mem.h"
#include "cleaner.h"

namespace scl
{
#define SCL_INTERFACE_ARGS \
	size_t _err_size, \
	err::err_t<_err_size> &_err, \
	\
	size_t _cleaner_size, \
	cleaner::cleaner_t<_cleaner_size> &_cleaner, \
	\
	mem::malloc_t _malloc, \
	mem::realloc_t _realloc, \
	mem::free_t _free, \
	\
	mem::mem_t<_malloc, _realloc, _free, _err_size, _err> &_mem

	template <
		size_t _err_size,
		err::err_t<_err_size> &_err,

		size_t _cleaner_size,
		cleaner::cleaner_t<_cleaner_size> &_cleaner,
		
		mem::malloc_t _malloc,
		mem::realloc_t _realloc,
		mem::free_t _free,
	
		mem::mem_t<_malloc, _realloc, _free, _err_size, _err> &_mem
	>
	struct scl_t
	{
		err::err_t<_err_size> &err = _err;
		cleaner::cleaner_t<_cleaner_size> &cleaner = _cleaner;
		mem::mem_t<_malloc, _realloc, _free, _err_size, _err> &mem = _mem;
	};


#define SCL_INTERFACE_DEFAULT_ARGS \
	err::default_array_size, err::default_err, \
	cleaner::default_array_size, cleaner::default_cleaner, \
	malloc, realloc, free, mem::default_mem


typedef scl_t<
		err::default_array_size, err::default_err,
		cleaner::default_array_size, cleaner::default_cleaner,
		malloc, realloc, free, mem::default_mem> default_scl_t;
	default_scl_t default_scl;
}