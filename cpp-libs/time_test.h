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

constexpr size_t size_of_test = 0x10FFFFFF;
constexpr size_t repeat = SIZE_MAX;

struct test
{
	int numbers[size_of_test];
	test(unsigned int a)
	{
		srand(a);
		for (size_t index = 0; index < size_of_test; index += 1)
			numbers[index] = rand();
	}

	constexpr size_t size() { return size_of_test; }
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
	return (duration_cast<cast_type>(end - start)).count();
}

// pass functions

int pass_by_pointer(test *pntr)
{
	int result = pntr->get(0);
	for (size_t index = 1; index < pntr->size(); index += 1)
	{
		result *= pntr->get(index);
	}

	return result;
}

int pass_by_reference(test value)
{
	int result = value[0];
	for (size_t index = 1; index < value.size(); index += 1)
	{
		result *= value[index];
	}
	return result;
}

int normal_pass(test value)
{
	int result = value[0];
	for (size_t index = 1; index < value.size(); index += 1)
	{
		result *= value[index];
	}
	return result;
}

// test functions

void test_pass_by_pointer()
{
	int result = pass_by_pointer(&data);
}

void test_normal_pass()
{
	int result = normal_pass(data);
}

void test_pass_by_reference()
{
	int result = pass_by_reference(data);
}

void timing()
{
	std::function<void()> func[]{
		test_normal_pass,
		test_pass_by_pointer,
		test_pass_by_reference,
	};

	// start the the time
	for (size_t index = 0; index < (sizeof(func) / sizeof(func[0])); index += 1)
	{
		std::clock_t c_start = std::clock();
		auto t_start = std::chrono::high_resolution_clock::now();

		std::thread t1(func[index]);
		std::thread t2(func[index]);
		t1.join();
		t2.join();

		std::clock_t c_end = std::clock();
		auto t_end = std::chrono::high_resolution_clock::now();

		std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
			<< 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
			<< "Wall clock time passed: "
			<< std::chrono::duration<double, std::milli>(t_end - t_start).count()
			<< " ms\n";
	}
}