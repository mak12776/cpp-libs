#pragma once

#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <chrono>
#include <functional>
#include <ctime>
#include <thread>

// test functions

constexpr size_t size_of_array = 3;
constexpr size_t repeat = SIZE_MAX;
const size_t number_of_threads = 3;


struct test
{
	int numbers[size_of_array];
	test(unsigned int a)
	{
		srand(a);
		for (size_t index = 0; index < size_of_array; index += 1)
			numbers[index] = rand();
	}

	constexpr size_t size() { return size_of_array; }
	constexpr inline int operator[](size_t index) { return numbers[index]; }
	constexpr inline int get(size_t index) { return numbers[index]; }
};

test data(3);

template <typename cast_type>
auto measure_time(void(*func)())
{
	auto start = std::chrono::high_resolution_clock::now();
	(*func)();
	auto end = std::chrono::high_resolution_clock::now();
	return (std::chrono::duration_cast<cast_type>(end - start)).count();
}

// pass functions

int pass_by_type_ref(test &value)
{
	int result = value[0];
	for (size_t index = 1; index < value.size(); index += 1)
	{
		result *= value[index];
	}
	return result;
}

int pass_by_type_pntr(test *pntr)
{
	int result = pntr->get(0);
	for (size_t index = 1; index < pntr->size(); index += 1)
	{
		result *= pntr->get(index);
	}

	return result;
}

int normal_pass_type(test value)
{
	int result = value[0];
	for (size_t index = 1; index < value.size(); index += 1)
	{
		result *= value[index];
	}
	return result;
}

int pass_by_value(int a, int b, int c)
{
	return a + b + c;
}

int pass_by_ref(int a, int b, int c)
{
	return a + b + c;
}

// test functions

void test_pass_by_pointer()
{
	int result = pass_by_type_pntr(&data);
}

void test_normal_pass()
{
	int result = normal_pass_type(data);
}

void test_pass_by_reference()
{
	int result = pass_by_type_ref(data);
}




void time_test()
{
	std::function<void()> functions[]{
		test_pass_by_reference,
		test_pass_by_pointer,
		test_normal_pass,
	};
	const size_t functions_size = sizeof(functions) / sizeof(functions[0]);
	std::thread threads[number_of_threads];

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