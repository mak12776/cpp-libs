// in the name of God.

// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "clib.h"
#include "scl.h"

//#include "counter.h"
//#include "winapi.h"



int main(int argc, const char **argv)
{
	printf("In The Name Of God.\n");

	io::set_default_width(20);
	io::print_separator();

	func<default_base_mem_t>();

	/*winapi::safe_set_current_priority_class(REALTIME_PRIORITY_CLASS);
	if (err::check())
	{
		printf("can't set process to HIGH_PRIORITY_CLASS\n");
		printf("error: %" PRIuDWORD "\n", GetLastError());
		return -1;
	}

	return bh::compress_main(argc, argv);*/

	/*for (int length = 1; length < argc; length += 1)
	{
		const char *name = argv[length];
		size_t size = io::get_file_name_size(name);
		if (err::check())
		{
			printf("error: stat() failed for '%s': errno: %s\n",
				name, strerror(errno));
			continue;
		}

		printf("file size for %s: %zu\n", name, size);
	}*/

	/*for (size_t index = 0; index < size; index += 1)
	{
		data_t value = rand();
		if (value == 0) value = 1;
		pntr[index] = value;
	}


	{
		winapi::auto_timer _;

		data_t new_value = 1;
		for (size_t step = 0; step < repeat; step += 1)
			for (size_t index = 0; index < size; index += 1)
				new_value *= pntr[index];
	}

	{
		winapi::auto_timer _;

		data_t new_value = 1;
		for (size_t step = 0; step < repeat; step += 1)
			for (data_t *beg = pntr, *end = beg + size; beg < end; beg += 1)
				new_value *= (*beg);
	}*/

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
