#pragma once

#include "err.h"
#include "winapi.h"
#include <stdexcept>

using namespace scl;

void test_exception_func1()
{
	throw std::bad_alloc();
}

void test_exception_func2()
{
	test_exception_func1();
}

void test_exception_func3()
{
	test_exception_func2();
}

void test_err_func1()
{
	err::set(err::MALLOC);
	err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
}

void test_err_func2()
{
	test_err_func1();

	if (err::check())
		err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
}

void test_err_func3()
{
	test_err_func2();

	if (err::check())
		err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
}

void test_main()
{
	const size_t repeat = 1'000'000'0;

	{
		winapi::auto_timer _;

		for (size_t len = 0; len < repeat; len += 1)
		{
			test_err_func3();

			if (err::check())
				err::clear();
		}
	}

	{
		winapi::auto_timer _;

		for (size_t len = 0; len < repeat; len += 1)
		{
			try
			{
				test_exception_func3();
			}
			catch (std::bad_alloc &) {}
		}
	}
}
