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
constexpr uint64_t repeat = UINT32_MAX;
const size_t number_of_threads = 0xFFFFF;


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

template <typename cast_type>
auto measure_time(void(*func)())
{
	auto start = std::chrono::high_resolution_clock::now();
	(*func)();
	auto end = std::chrono::high_resolution_clock::now();
	return (std::chrono::duration_cast<cast_type>(end - start)).count();
}

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


// simple time test
std::thread threads[number_of_threads];

void time_test()
{

	/*
	D:\Codes\cpp-libs\x64\Release>cpp-libs.exe
	CPU time used: 142871.00 ms
	Wall clock time passed: 142870.83 ms
	CPU time used: 143784.00 ms
	Wall clock time passed: 143783.32 ms
	CPU time used: 141045.00 ms
	Wall clock time passed: 141045.57 ms

	D:\Codes\cpp-libs\x64\Release>cpp-libs.exe
	CPU time used: 141762.00 ms
	Wall clock time passed: 141761.30 ms
	CPU time used: 139719.00 ms
	Wall clock time passed: 139718.57 ms
	CPU time used: 143051.00 ms
	Wall clock time passed: 143050.92 ms
	*/

	std::function<void()> functions[]{
		test_normal_pass,
		test_pass_by_reference,
		test_pass_by_pointer,
		
	};
	const size_t functions_size = sizeof(functions) / sizeof(functions[0]);
	

	// start the the time
	for (size_t index = 0; index < functions_size; index += 1)
	{
		std::clock_t c_start = std::clock();
		auto t_start = std::chrono::high_resolution_clock::now();

		for (size_t threads_index = 0; threads_index < number_of_threads; threads_index += 1)
			threads[threads_index] = std::thread(functions[index]);

		for (size_t threads_index = 0; threads_index < number_of_threads; threads_index += 1)
			threads[threads_index].join();

		std::clock_t c_end = std::clock();
		auto t_end = std::chrono::high_resolution_clock::now();

		auto diff = t_end - t_start;
		auto duration = std::chrono::duration<uint64_t, std::nano>(t_end - t_start);

		std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
			<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
			<< "Wall clock time passed: "
			<< std::chrono::duration<double, std::milli>(t_end - t_start).count()
			<< " ms\n";
	}
}