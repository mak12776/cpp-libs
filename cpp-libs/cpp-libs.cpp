// in the name of God.

// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
// "in the name of God"

#include "pch.h"

#include <iostream>
#include "bith.h"

template <typename int_type, typename float_type, bool print_info = false>
bool test_integer(int_type value)
{
	int_type result = (int_type)((float_type)value);
	if constexpr (print_info)
		if (result != value)
			std::cout 
				<< "failed, value: " << value << " != " << result << std::endl;
	return result != value;
}

template <typename int_type, typename float_type>
void test_range()
{
	int_type min = (std::numeric_limits<int_type>::min)();
	int_type max = (std::numeric_limits<int_type>::max)();

	std::cout 
		<< "test_rage<" << typeid(float_type).name()
		<< ", " << typeid(int_type).name() << ">  "
		<< "digits: " << std::numeric_limits<float_type>::digits
		<< ", " << std::numeric_limits<int_type>::digits
		<< std::endl;

	for (int_type value = min; value < max; value += 1)
	{
		if (test_integer<float_type, int_type>(value))
		{
			std::cout << "first value: " << value << std::endl;
			break;
		}
	}

	for (int_type value = max; value > min; value -= 1)
	{
		if (test_integer<float_type, int_type>(value))
		{
			std::cout << "last value: " << value << std::endl;
			break;
		}
	}
}

int main(int argc, const char **argv)
{
	bith::segment_buffer_t segmented_buffer;
	bith::count_bits(12, "D:\\Codes\\data-examples\\music_for_programming_49-julien_mier.mp3", segmented_buffer);
	scl::err::check_print_exit();

	// ---------------------------

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
