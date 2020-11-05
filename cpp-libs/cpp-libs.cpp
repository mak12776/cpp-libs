// in the name of God.

// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
// "in the name of God"

#include "pch.h"
#include "scl/all.h"

#include "programs/bh.h"

#include <cstdio>
#include <Windows.h>
#include <filesystem>
#include <iostream>

namespace ncl = scl;


template <typename data_type>
struct simple_data : scl::io::cache_t
{
	size_t size;
	data_type *pntr;

	simple_data(size_t size)
	{ 
		using namespace scl;

		pntr = mem::safe_malloc_array<data_type>(size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;
		this->size = size;
	}

	virtual bool read(FILE *file)
	{
		using namespace scl;

		ubyte size_of_size;
		ubyte size_of_data;

		// size of size

		io::safe_fread_data<ubyte>(size_of_size, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;
		
		if (size_of_size != sizeof(size_t))
		{
			err::set(err::INVALID_FILE_STRUCTURE);
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return true;
		}

		// size of data

		io::safe_fread_data<ubyte>(size_of_data, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		if (size_of_data != sizeof(data_type))
		{
			err::set(err::INVALID_FILE_STRUCTURE);
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return true;
		}

		// size

		io::safe_fread_data(size, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		// malloc memory

		pntr = mem::safe_malloc_array<data_type>(size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		// data array

		for (size_t index = 0; index < size; index += 1)
		{
			io::safe_fread_data<data_type>(pntr[index], file);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return true;
		}

		return false;
	}

	virtual bool write(FILE *file)
	{
		using namespace scl;

		ubyte size_of_size = sizeof(size_t);
		ubyte size_of_data = sizeof(data_type);

		// size of size

		io::safe_fwrite_data<ubyte>(size_of_size, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		// size of data

		io::safe_fwrite_data<ubyte>(size_of_data, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		// size

		io::safe_fwrite_data(size, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		// data array

		for (size_t index = 0; index < size; index += 1)
		{
			io::safe_fwrite_data<data_type>(pntr[index], file);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return true;
		}

		return false;
	}
};

template <typename data_type>
bool find_primes(simple_data<data_type> primes)
{
	if (primes.size == 0)
		return;
	
	primes.pntr[0] = 2;

	data_type next_prime = 3;
	size_t index = 1;
	while (index < primes.size)
	{
		bool is_prime = true;
		for (size_t check_index = 0; check_index < index; check_index += 1)
		{
			if (next_prime % primes.pntr[index] == 0)
			{
				is_prime = false;
				break;
			}
		}

		if (is_prime)
			primes.pntr[index++] = next_prime;
		next_prime += 2;
	}

	return false;
}


int main(int argc, const char **argv)
{
	namespace fs = std::filesystem;

	printf("in the name of God\n");
	scl::cfmt::print_separator(20, '=');

	simple_data<int> numbers(200);

	scl::io::cached_function("primes.data", nullptr, numbers);

	scl::cfmt::printf_ln("Hello %s!", "Amin");

	/*scl::io::file_cache_t file_cache("cache.txt");*/

	/*scl::c_string_t data_folder("D:\\Codes\\data-examples");
	scl::c_string_t file_name("music_for_programming_49-julien_mier.mp3");

	bh::count_file_name<8>(data_folder, file_name);
	err::write_exit();*/

	/*scl::c_string_t name("Amin");
	scl::m_string_t sent;

	sent.malloc_string("Hello ");
	if (scl::err::check())
		return -1;

	printf("%s\n", sent.pntr);
	printf("size: %zu, len: %zu\n", sent.size, sent.len);

	sent.realloc_cat(name);
	if (scl::err::check())
		return -1;

	printf("%s\n", sent.pntr);
	printf("size: %zu, len: %zu\n", sent.size, sent.len);*/

	/*c_string_t file_name("D:\\Codes\\cpp-libs\\cpp-libs\\Hello.txt");

	counter::counter_t counter;
	io::fopen_fread_buffered(file_name.pntr, counter);

	if (err::check())
	{
		printf("err: %s\n", err::string());
		if (err::test(err::FOPEN))
			printf("errno: %s\n", strerror(errno));
		return 1;
	}

	printf("%s: %zu\n", "lines", counter.lines);
	printf("%s: %zu\n", "words", counter.words);
	printf("%s: %zu\n", "digits", counter.digits);
	printf("%s: %zu\n", "symbols", counter.symbols);*/

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
