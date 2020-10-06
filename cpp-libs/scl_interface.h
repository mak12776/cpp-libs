#pragma once

#include "err.h"
#include "mem.h"
#include "cleaner.h"

namespace scl
{

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


typedef scl_t<
		err::default_array_size, err::default_err,
		cleaner::default_array_size, cleaner::default_cleaner,
		malloc, realloc, free, mem::default_mem> default_scl_t;
	default_scl_t default_scl;

	// 
#define SCL_TEMPLATE_ARGS_EXT(PRE, NAME) \
	size_t PRE ## err_size, \
	scl::err::err_t<_err_size> &PER ## err, \
	\
	size_t PRE ## cleaner_size, \
	scl::cleaner::cleaner_t<PRE ## cleaner_size> &PER ##cleaner, \
	\
	scl::mem::malloc_t PER ## malloc, \
	scl::mem::realloc_t PER ## realloc, \
	scl::mem::free_t PRE ## free, \
	scl::mem::mem_t< \
		PRE ## malloc, PRE ## realloc, PRE ## free, \
		PRE ## err_size, PRE ## err> &PRE ## mem \
	\
	scl::scl_t< \
		PRE ## err_size, PRE ## err, \
		PRE ## cleaner_size, PRE ## cleaner, \
		PRE ## malloc, PRE ## realloc, PRE ## free, PRE ## mem> NAME

#define SCL_TEMPLATE_ARGS(NAME) SCL_TEMPLATE_ARGS_EXT(__xx__, NAME)
}