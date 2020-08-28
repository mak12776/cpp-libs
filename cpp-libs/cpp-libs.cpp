// cpp-libs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define SCL_USE_ERROR

#include "scl.h"

#include <array>
#include <inttypes.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <thread>

using namespace scl;

struct buffer_t
{
	ubyte *pntr;
	size_t size;
};

enum class block_flag : uint8_t
{
	DIFFERENT, SIMILAR
};

struct block_t
{
	block_flag flag;
	ubyte *start;
	ubyte *end;
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

// array structure
template <typename data_type, size_t array_size>
struct array
{
	typedef size_t size_type;
	typedef data_type &reference;
	typedef const data_type &const_reference;

	data_type pntr[array_size];

	inline reference operator[](size_type index)
	{
		return pntr[index];
	}

	inline const_reference operator[](size_type index)
	{
		return pntr[index];
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

// const_string

template <const char *value>
struct const_string
{
	
};

// sequence counts

struct sequence_counts
{
	size_t first_offset;
	linked_array_node<size_t> other_offsets;
};

// process clean up

enum class clean_up_type: uint8_t
{
	FILE_CLOSE, MEMORY_FREE,
};

struct clean_up_job
{
	clean_up_type type;
	union
	{
		FILE *file;
		void *pntr;
	};
};

constexpr size_t max_clean_ups = 1024;
size_t clean_ups_index = 0;
clean_up_job clean_ups[max_clean_ups];

void push_file_clean_up(FILE *file)
{
	if (clean_ups_index != max_clean_ups)
	{
		clean_ups[clean_ups_index].type = clean_up_type::FILE_CLOSE;
		clean_ups[clean_ups_index].file = file;
		clean_ups_index += 1;
	}
}

void push_memory_clean_up(void *pntr)
{
	if (clean_ups_index != max_clean_ups)
	{
		clean_ups[clean_ups_index].type = clean_up_type::MEMORY_FREE;
		clean_ups[clean_ups_index].pntr = pntr;
		clean_ups_index += 1;
	}
}

void clean_up_finish()
{
	for (size_t index = 0; index < clean_ups_index; index += 1)
	{
		switch (clean_ups[index].type)
		{
		case clean_up_type::FILE_CLOSE:
			fclose(clean_ups[index].file);
			break;

		case clean_up_type::MEMORY_FREE:
			free(clean_ups[index].pntr);
			break;
		}
	}
}

// print tools

size_t DEFAULT_WIDTH = 80;
void set_default_width(size_t width) { DEFAULT_WIDTH = width; }
void reset_default_width() { DEFAULT_WIDTH = 80; }

size_t print_line(size_t width = 0, ubyte character = '-', FILE *output_file = stdout)
{
	ubyte *buffer;
	size_t write_number;

	if (width == 0) width = DEFAULT_WIDTH;

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

// compress main function

void searching_biggest_repetitive_sequences(buffer_t buffer)
{
	// trying to find biggest repetitive sequences:
	printf("trying to find biggest repetitive sequences:\n");

	bool found = false;

	size_t len = buffer.size + 1;

	ubyte *base_pntr = buffer.pntr;
	ubyte *check_pntr = base_pntr + 1;

	ubyte *max_base_pntr = buffer.pntr + 1;
	ubyte *max_check_pntr = buffer.pntr + 2;

	size_t number_of_success = 0;

	for (; len >= 2; len -= 1)
	{
		printf("len: %zu\n", len);
		for (base_pntr = buffer.pntr; base_pntr < max_base_pntr; base_pntr += 1)
		{
			for (check_pntr = base_pntr + 1; check_pntr < max_check_pntr; check_pntr += 1)
			{
				// printf("\t%" PRIuPTR "\t%" PRIuPTR "\n", base_pntr, check_pntr);
				if (std::memcmp(base_pntr, check_pntr, len) == 0)
				{
					number_of_success += 1;
					found = true;
				}
			}

			if (found)
				break;
		}
		if (found)
			break;

		max_check_pntr += 1;
		max_base_pntr += 1;

	}

	if (!found)
		printf("offsets:\n\t%08xp\n\t%08xp\nlen: %zu\n", base_pntr, check_pntr, len);
	else
		printf("not found.\n");
}

// counting bytes

constexpr size_t counts_size = 256;
void counting_bytes(buffer_t *buffer, size_t counts[counts_size], size_t *zero_counts_number)
{
	printf("counting bytes: ");
	*zero_counts_number = 0;

	for (size_t index = 0; index < counts_size; index += 1)
		counts[index] = 0;

	// add to counts for each byte
	for (size_t index = 0; index < buffer->size; index += 1)
		counts[buffer->pntr[index]] += 1;

	for (size_t index = 0; index < counts_size; index += 1)
		if (counts[index] == 0)
			*zero_counts_number += 1;
	printf("done\n");
}

void sorting_bytes(buffer_t *buffer, size_t counts[counts_size], byte_count_t byte_counts[counts_size])
{
	printf("sorting bytes: ");

	for (size_t index = 0; index < counts_size; index += 1)
	{
		byte_counts[index].value = (ubyte)index;
		byte_counts[index].count = counts[index];
	}

	std::qsort(byte_counts, counts_size, sizeof(byte_count_t), byte_count_t::compare);

	printf("done\n"); // sorting bytes
}

// printf functions

size_t printf_file_info(const char *file_name, size_t file_size)
{
	size_t write_number;
	write_number = printf("File Name: %s\n", file_name);
	write_number += printf("File Size: %zu\n", file_size);
	return write_number;
}

size_t print_info(ubyte *pntr1, ubyte *pntr2, size_t size)
{
	static const size_t PTR_HEX_WIDTH = sizeof(ubyte *) * 2;
	static ubyte line_buffer[PTR_HEX_WIDTH * 2 + 20];

	// TODO: incomplete

	return 0;
}

const char *info_file_ext = ".temp-info";
const size_t info_file_ext_size = strlen(info_file_ext);

void compress_file(const char *file_name)
{
	buffer_t buffer;
	FILE *info_file;

	const size_t file_name_size = strlen(file_name);
	const size_t info_file_name_size = file_name_size + info_file_ext_size;

	// read entire file into buffer
	io::read_file_name(file_name, (void **)&(buffer.pntr), &buffer.size);

	// allocating memory for info file name
	char *info_file_name = (char *)memory::safe_malloc(info_file_name_size + 1);
#ifdef SCL_USE_ERROR
	if (error::check()) return;
#endif

	// creating info file name
	memcpy((void *)info_file_name, file_name, strlen(file_name));
	memcpy((void *)(info_file_name + file_name_size), info_file_ext, info_file_ext_size);
	info_file_name[info_file_name_size] = '\0';

	// opern info file for writing
	info_file = io::safe_fopen(info_file_name, "wb");
#ifdef SCL_USE_ERROR
	if (error::check()) return;
#endif

	printf("info file created: %s\n", info_file_name);
	print_line();


	if (error::check())
	{


		printf("something happend: %s\n", error::get_last_error());
		if (error::errors[0].num == error::num_t::ERROR_FOPEN)
			printf("fopen error: %s\n", strerror(errno));
		return;
	}

	set_default_width(20);

	printf_file_info(file_name, buffer.size);
	print_line();

	size_t counts[counts_size];
	size_t zero_counts_number = 0;
	byte_count_t byte_counts[counts_size];
	size_t maximum_count;

	counting_bytes(&buffer, counts, &zero_counts_number);
	sorting_bytes(&buffer, counts, byte_counts);

	print_line();

	// log some informations
	maximum_count = byte_counts[counts_size - 1].count;

	printf("zero counts number: %zu\n", zero_counts_number);
	printf("maximum count: %zu (%.1f%%)\n", maximum_count, ((double)maximum_count / (double)buffer.size) * 100);

	print_line();

	// breaking the file:
	// printf("breaking up the file:");

	size_t *offsets = memory::new_array<size_t>(maximum_count);
	if (error::check())
	{
		printf("error: can't allocate %zu offsets.\n", maximum_count);
		return;
	}

	ubyte value = 0;
	size_t offsets_index = 0;
	for (size_t offset = 0; offset < buffer.size; offset += 1)
	{
		if (buffer.pntr[offset] == value)
		{
			offsets[offsets_index] = offset;
			offsets_index += 1;
		}
	}

	reset_default_width();
}

int compress_main(int argc, const char **argv)
{
	if (argc == 1)
	{
		printf("usage: %s [FILENAME]\n", argv[0]);
		return 0;
	}

	compress_file(argv[1]);
	for (int index = 2; index < argc; index += 1)
	{
		print_line(0, '=');
		compress_file(argv[index]);
	}

	return 0;
}


int main(int argc, const char **argv)
{
	return compress_main(argc, argv);
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
