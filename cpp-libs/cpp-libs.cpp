// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define SCL_USE_ERROR

#include "scl.h"
#include <inttypes.h>
#include <stdlib.h>
#include <fcntl.h>

using namespace scl;

struct buffer_t
{
	ubyte *pntr;
	size_t size;
};

struct byte_count_t
{
	ubyte value;
	size_t count;

	static int compare(const void *a, const void *b)
	{
		const byte_count_t arg1 = *static_cast<const byte_count_t *>(a);
		const byte_count_t arg2 = *static_cast<const byte_count_t *>(b);

		if (arg1.count < arg2.count) return -1;
		if (arg1.count > arg2.count) return 1;
		return 0;
	}
};

// linked array structure

template <typename data_type>
struct linked_array_node
{
	data_type *pntr;
	linked_array_node<data_type> *next;

	linked_array_node(size_t size)
	{
		this->pntr = memory::new_array<data_type>(size);
		this->next = nullptr;
	}

	linked_array_node(size_t size, linked_array_node<data_type> *next)
	{
		this->pntr = memory::new_array<data_type>(size);
		this->next = next;
	}
};

template <typename data_type>
struct linked_array
{
	size_t array_size;
	size_t array_length;
	size_t length;
	linked_array_node<data_type> *first;
	linked_array_node<data_type> *last;

	linked_array(size_t array_size)
	{
		this->array_size = array_size;
		this->array_length = 0;
		this->length = 0;
		this->first = nullptr;
		this->last = nullptr;
	}

	void append(data_type value)
	{
		if (this->first == nullptr)
		{
			this->last = (this->first = new linked_array_node(array_size));
			this->first->pntr[0] = value;
			this->array_size = 1;
		}
		else if (this->array_size == this->array_length)
		{
			this->last = new linked_array_node(array_size, this->last);
			this->last->pntr[0] = value;
			this->array_size = 1;
		}
		else
		{
			this->last->pntr[this->array_size] = value;
			this->array_size += 1;
		}
		this->length += 1;
	}
};

// sequence counts

struct sequence_counts
{
	size_t first_offset;
	linked_array_node<size_t> other_offsets;
};

// print tools

constexpr size_t DEFAULT_WIDTH = 80;
ubyte line_buffer[DEFAULT_WIDTH];

size_t print_line(size_t width = DEFAULT_WIDTH, ubyte character = '-', FILE *output_file = stdout)
{
	ubyte *buffer;
	size_t write_number;

	buffer = memory::new_array<ubyte>(width + 1);
#ifdef SCL_USE_ERROR
	if (scl::error::check()) return 0;
#endif

	for (size_t index = 0; index < width; index += 1)
		buffer[index] = character;
	buffer[width++] = '\n';

	write_number = fwrite(buffer, 1, width, output_file);
	memory::free(buffer);

	return write_number;
}

// main function

int main(int argc, const char **argv)
{
	const char *file_name;
	if (argc != 2)
	{
		printf("usage: %s [FILENAME]\n", argv[0]);
		return 0;
	}
	file_name = argv[1];

	buffer_t buffer;

	io::read_file_name(file_name, (void **)&(buffer.pntr), &buffer.size);

	printf("File Name: %s\n", file_name);
	printf("File Size: %zu\n", buffer.size);

	if (error::check())
	{
		printf("something happend: %s\n", error::get_last_error());
		return 1;
	}

	// now try to compress the file with the limits

	print_line(20);
	printf("counting bytes: ");

	constexpr size_t counts_size = 256;
	size_t counts[counts_size];
	size_t zero_counts_number = 0;

	// zero all counts
	for (size_t index = 0; index < counts_size; index += 1)
		counts[index] = 0;

	// add to counts for each byte
	for (size_t index = 0; index < buffer.size; index += 1)
		counts[buffer.pntr[index]] += 1;

	for (size_t index = 0; index < counts_size; index += 1)
		if (counts[index] == 0) 
			zero_counts_number += 1;

	printf("done\n");

	printf("sorting bytes: ");

	// store counts in byte_count_t array
	byte_count_t byte_counts[counts_size];
	for (size_t index = 0; index < counts_size; index += 1)
	{
		byte_counts[index].value = index;
		byte_counts[index].count = counts[index];
	}

	// qsort all byte counts
	std::qsort(byte_counts, counts_size, sizeof(byte_count_t), byte_count_t::compare);

	size_t maximum_count = byte_counts[counts_size - 1].count;
	size_t minimum_count = byte_counts[0].count;

	printf("done\n");

	print_line(10);

	printf("zero counts number: %zu\n", zero_counts_number);
	printf("maximum count: %zu (%.1f%%)\n", maximum_count, ((double)maximum_count / (double)buffer.size) * 100);

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
