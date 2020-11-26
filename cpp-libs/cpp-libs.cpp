// in the name of God.

// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
// "in the name of God"

#include "pch.h"

#include "scl/all.h"
#include <thread>

int main(int argc, const char **argv)
{
	scl::c_string_t file_path("D:\\Codes\\data-examples\\music_for_programming_48-michael_hicks.mp3");
	scl::ubuffer_t buffer;

	buffer.allocate_fopen_fread(file_path.pntr);
	scl::err::print_exit();

	unsigned int hardware_concurrency = std::thread::hardware_concurrency();

	if (hardware_concurrency <= 0)
	{
		printf("error: invalid hardware concurrency: %i\n", hardware_concurrency);
		return 1;
	}



	// ---------------------------
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
