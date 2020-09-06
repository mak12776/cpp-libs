#pragma once

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <chrono>
#include <functional>
#include <ctime>
#include <thread>

// test functions

constexpr size_t size_of_array = 0xFFF;
constexpr uint64_t repeat = UINT32_MAX / 4;

struct test_data
{
	int a, b, c, d, e;
	test_data(unsigned int num)
	{
		srand(num);
		a = rand();
		b = rand();
		c = rand();
		d = rand();
		e = rand();
	}
};

test_data data(3);

// pass functions
int pass_by_type_reference(test_data &value)
{
	return value.a + value.b + value.c + value.d + value.e;
}

int pass_by_type_pointer(test_data *pntr)
{
	return pntr->a + pntr->b + pntr->c + pntr->d + pntr->e;
}

int normal_type_pass(test_data value)
{
	return value.a + value.b + value.c + value.d + value.d;
}

// test functions
inline void test_pass_by_pointer()
{
	for (uint64_t step = 0; step < repeat; step += 1)
		int result = pass_by_type_pointer(&data);

}

inline void test_normal_pass()
{
	for (uint64_t step = 0; step < repeat; step += 1)
		int result = normal_type_pass(data);
}

inline void test_pass_by_reference()
{
	for (uint64_t step = 0; step < repeat; step += 1)
		int result = pass_by_type_reference(data);
}

void time_test()
{
	printf("-- normal pass\n");
	{
		winapi::auto_timer _;
		test_normal_pass();
	}


	printf("-- pass by pointer\n");

	{
		winapi::auto_timer _;
		test_pass_by_pointer();
	}

	printf("-- pass by reference\n");

	{
		winapi::auto_timer _;
		test_pass_by_reference();
	}
}
