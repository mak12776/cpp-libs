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

	/*
	working example:

	size_t _err_size = scl::err::default_array_size,
	scl::err::err_t<_err_size> &_err = scl::err::default_err,

	size_t _cleaner_size = scl::cleaner::default_array_size,
	scl::cleaner::cleaner_t<_cleaner_size> &_cleaner =
		scl::cleaner::default_cleaner,

	scl::mem::malloc_t _malloc = malloc,
	scl::mem::realloc_t _realloc = realloc,
	scl::mem::free_t _free = free,
	scl::mem::mem_t<_malloc, _realloc, _free, _err_size, _err> &_mem =
		scl::mem::default_mem,

	scl::scl_t<
		_err_size, _err, _cleaner_size, _cleaner,
		_malloc, _realloc, _free, _mem> &ncl = scl::default_scl
	*/

#define SCL_TEMPLATE_ARGS_EXT(PRE_FIX, NAME) \
	size_t PRE_FIX ## err_size = scl::err::default_array_size, \
	scl::err::err_t<PRE_FIX ## err_size> &PRE_FIX ## err = \
		scl::err::default_err, \
	\
	size_t PRE_FIX ## cleaner_size = scl::cleaner::default_array_size, \
	scl::cleaner::cleaner_t<PRE_FIX ## cleaner_size> &PRE_FIX ## cleaner = \
		scl::cleaner::default_cleaner, \
	\
	scl::mem::malloc_t PRE_FIX ## malloc = malloc, \
	scl::mem::realloc_t PRE_FIX ## realloc = realloc, \
	scl::mem::free_t PRE_FIX ## free = free, \
	\
	scl::mem::mem_t< \
		PRE_FIX ## malloc, PRE_FIX ## realloc, PRE_FIX ## free, \
		PRE_FIX ## err_size, PRE_FIX ## err> \
		&PRE_FIX ## mem = scl::mem::default_mem, \
	\
	scl::scl_t< \
		PRE_FIX ## err_size, PRE_FIX ## err, \
		PRE_FIX ## cleaner_size, PRE_FIX ## cleaner, \
		PRE_FIX ## malloc, PRE_FIX ## realloc, PRE_FIX ## free, \
		PRE_FIX ## mem> &NAME = scl::default_scl

#define SCL_TEMPLATE_ARGS(NAME) SCL_TEMPLATE_ARGS_EXT(__xx__xx__, NAME)
#define SCL_TEMPLATE_ARGS_NCL SCL_TEMPLATE_ARGS(ncl)

}