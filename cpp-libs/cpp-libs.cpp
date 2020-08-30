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

template <typename data_type>
struct data_count_t
{
	data_type data;
	size_t count;
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


// sequence counts

struct sequence_counts
{
	size_t first_offset;
	linked_array_node<size_t> other_offsets;
};

// process clean up

enum class clean_up_type: uint8_t
{
	FILE_CLOSE, MEMORY_FREE, MEMORY_DELETE,
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

void add_file_clean_up(FILE *file)
{
	if (clean_ups_index != max_clean_ups)
	{
		clean_ups[clean_ups_index].type = clean_up_type::FILE_CLOSE;
		clean_ups[clean_ups_index].file = file;
		clean_ups_index += 1;
	}
}

void add_memory_free_clean_up(void *pntr)
{
	if (clean_ups_index != max_clean_ups)
	{
		clean_ups[clean_ups_index].type = clean_up_type::MEMORY_FREE;
		clean_ups[clean_ups_index].pntr = pntr;
		clean_ups_index += 1;
	}
}

void add_memory_delete_clean_up(void *pntr)
{
	if (clean_ups_index != max_clean_ups)
	{
		clean_ups[clean_ups_index].type = clean_up_type::MEMORY_DELETE;
		clean_ups[clean_ups_index].pntr = pntr;
		clean_ups_index += 1;
	}
}

void clean_ups_finish()
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

		case clean_up_type::MEMORY_DELETE:
			delete clean_ups[index].pntr;
			break;
		}
	}
}

// string tools

template<typename value_type = char>
struct basic_sstring
{
	const value_type *pntr;
	const size_t size;

	basic_sstring(const value_type *pntr) : pntr(pntr), size(strlen(pntr))
	{ }

	basic_sstring(const value_type *pntr, const size_t size) : pntr(pntr), size(size)
	{ }
};

using sstring = basic_sstring<char>;

struct c_string_t
{
	const char *const pntr;
	const size_t len;

	c_string_t(const char *pntr) : pntr(pntr), len(strlen(pntr))
	{ }
};

struct m_string_t
{
	char *pntr;
	size_t len;
	size_t size;
	
	m_string_t(size_t size)
	{
		size_t size;

		this->pntr = (char *)memory::safe_malloc(size);
		if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
			return;
		}

		this->pntr[0] = '\0';
		this->len = 0;
		this->size = size;
	}

	m_string_t(c_string_t *string1, c_string_t *string2)
	{
		size_t size;

		math::safe_add(string1->len, string2->len, size);
		if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
			return;
		}

		math::safe_add(size, (size_t)1, size);
		if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
			return;
		}

		this->pntr = (char *)memory::safe_malloc(size);
		if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
			return;
		}

		memcpy(this->pntr, string1->pntr, string1->len);
		memcpy((void *)(this->pntr + string1->len), string2->pntr, string2->len);
		this->pntr[size] = '\0';
		this->len = string1->len + string2->len;
		this->size = size;
	}
};

static inline sstring concat(sstring *string1, sstring *string2)
{
	size_t size = string1->size + string2->size;
	char *pntr = (char *)memory::safe_malloc(size + 1);

	if (err::check())
		return sstring(nullptr, 0);

	memcpy((void *)pntr, string1->pntr, string1->size);
	memcpy((void *)(pntr + string1->size), string2->pntr, string2->size);
	pntr[size] = '\0';

	return sstring(pntr, size);
}

// print tools

template <typename value_type>
size_t print_size_of()
{
	return printf("sizeof %s: %zu\n", typeid(value_type).name(), sizeof(value_type));
}

template <typename value_type>
size_t print_address_of(value_type *value, const char *name)
{
	return printf("address of %s: %zu\n", name, value);
}

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
	if (scl::err::check()) return 0;
#endif

	for (size_t index = 0; index < width; index += 1)
		buffer[index] = character;
	buffer[width++] = '\n';

	write_number = fwrite(buffer, 1, width, output_file);
	memory::free(buffer);

	return write_number;
}

// compress main function

void searching_biggest_repetitive_sequences(ubuffer_t buffer)
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
		printf("offsets:\n\t%08p\n\t%08p\nlen: %zu\n", base_pntr, check_pntr, len);
	else
		printf("not found.\n");
}

// counting bytes

constexpr size_t counts_size = 256;
void counting_bytes(ubuffer_t *buffer, size_t counts[counts_size], size_t *zero_counts_number)
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

void sorting_bytes(ubuffer_t *buffer, size_t counts[counts_size], byte_count_t byte_counts[counts_size])
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

// const char *info_file_ext = ".temp-info";
// const size_t info_file_ext_size = strlen(info_file_ext);

sstring info_file_ext(".temp-info");

// search offsets

void search_offsets(const char *_file_name)
{
	ubuffer_t buffer;
	FILE *info_file;

	sstring file_name(_file_name);

	sstring info_file_name = concat(&file_name, &info_file_ext);
	if (err::check()) return;

	add_memory_free_clean_up((void *)info_file_name.pntr);

	// read entire file into buffer
	io::read_file_name(file_name.pntr, (void **)&(buffer.pntr), &buffer.size);
	if (err::check())
	{
		printf("error: %s\n", err::get_string());
		if (err::num == err::FOPEN) 
			printf("fopen error: %s\n", strerror(errno));
		return;
	}

	// opern info file for writing
	info_file = io::safe_fopen(info_file_name.pntr, "wb");
	if (err::check()) return;

	add_file_clean_up(info_file);

	// log info file name
	set_default_width(20);

	printf("info file created: %s\n", info_file_name.pntr);
	print_line();

	printf_file_info(file_name.pntr, buffer.size);
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
	if (err::check())
	{
		printf("error: can't allocate %zu offsets.\n", maximum_count);
		clean_ups_finish();
		return;
	}
	add_memory_delete_clean_up(offsets);

	io::fwrite_all(counts, sizeof(counts), info_file);
	if (err::check())
	{
		printf("error: while writing counts on info file: %s\n", strerror(errno));
		clean_ups_finish();
		return;
	}

	ubyte value = 0;
	do
	{
		size_t offsets_index = 0;
		for (size_t offset = 0; offset < buffer.size; offset += 1)
		{
			if (buffer.pntr[offset] == value)
			{
				offsets[offsets_index] = offset;
				offsets_index += 1;
			}
		}

		io::fwrite_all(offsets, sizeof(offsets[0]) * offsets_index, info_file);
		if (err::check())
		{
			printf("error: while writing offsets of %hhx on info file: %s\n", value, strerror(errno));
			clean_ups_finish();
			return;
		}

		value += 1;
	} while (value != 0);
	
	reset_default_width();
}

// offsets check

void offsets_check(const char *_file_name)
{
	constexpr size_t counts_size = 256;
	size_t counts[counts_size];
	size_t info_counts[counts_size];

	sstring file_name(_file_name);

	sstring info_file_name = concat(&file_name, &info_file_ext);

	if (err::check())
	{
		printf("error: can't allocate memory for info_file_name\n");
		return;
	}
	add_memory_free_clean_up((void *)info_file_name.pntr);

	ubuffer_t buffer;

	io::read_file_name(file_name.pntr, (void **)&buffer.pntr, &buffer.size);
	if (err::check())
	{
		printf("error: can't read input file: %s\n", file_name.pntr);
		printf("errno: %s\n", strerror(errno));
		clean_ups_finish();
		return;
	}
	add_memory_free_clean_up(buffer.pntr);

	FILE *info_file = io::safe_fopen(info_file_name.pntr, "rb");
	if (err::check())
	{
		printf("error: can't fopen: %s\n", info_file_name.pntr);
		printf("errno: %s\n", strerror(errno));
		clean_ups_finish();
		return;
	}
	add_file_clean_up(info_file);

	for (size_t index = 0; index < counts_size; index += 1)
		counts[index] = 0;

	for (size_t index = 0; index < buffer.size; index += 1)
		counts[buffer.pntr[index]] += 1;

	io::safe_fread((void *)info_counts, sizeof(info_counts), info_file);
	if (err::check())
	{
		printf("error: can't read info file: %s\n", info_file_name.pntr);
		printf("errno: %s\n", strerror(errno));
		clean_ups_finish();
		return;
	}

	if (memcmp(counts, info_counts, sizeof(counts)) != 0)
	{
		printf("offsets check: invalid counts\n");
		clean_ups_finish();
		return;
	}

	size_t offsets_size = *std::max_element(info_counts, info_counts + counts_size);

	printf("max counts: %zu\n", offsets_size);

	size_t *offsets = memory::new_array<size_t>(offsets_size);
	if (err::check())
	{
		printf("error: can't allocate memory for %zu offsets\n", offsets_size);
		return;
	}
	add_memory_delete_clean_up(offsets);

	ubyte value = 0;
	do
	{
		io::fread_all(offsets, sizeof(offsets[0]) * counts[value], info_file);
		if (err::check())
		{
			printf("error: while reading offsets of %hhu: %s", value, strerror(errno));
			clean_ups_finish();
			return;
		}

		
		for (size_t index = 0; index < counts[value]; index += 1)
		{
			if (offsets[index] > buffer.size)
			{
				printf("offsets check: offset for %hhu at %zu is bigger than size: %zu\n", value, index, buffer.size);
				clean_ups_finish();
				return;
			}

			if (buffer.pntr[offsets[index]] != value)
			{
				printf("offsets check: offset for %hhu at %zu dosn't match ubyte at %zu on input file '%s'\n", value, index, offsets[index], buffer.pntr[offsets[index]]);
				clean_ups_finish();
				return;
			}
		}

		printf("offsets check: %02hhx successfull.\n", value);

		value += 1;
	} while (value != 0);

	printf("everything successfull.\n");
	clean_ups_finish();
	return;
}

// count random-bit-width

void count_bit_width(const char *file_name)
{
	ubuffer_t buffer;
	const size_t file_name_size = strlen(file_name);

	// read input file
	io::read_file_name(file_name, (void **)&buffer.pntr, &buffer.size);
	if (err::check())
	{
		printf("error: can't read %s: %s\n", file_name, err::get_string());
		printf("errno: %s\n", std::strerror(errno));
		return;
	}
	add_memory_free_clean_up(buffer.pntr);

	// count 64bits
	using value_type = uint64_t;

	std::vector<data_count_t<value_type>> data_counts(1024);
	size_t data_counts_index = 0;

	const size_t max_buffer_index = buffer.size / 8;
	size_t *pntr = (size_t *)buffer.pntr;

	for (size_t index = 0; index < max_buffer_index; index += 1)
	{
		value_type value = pntr[index];
		std::vector<data_count_t<value_type>>::iterator search = std::find_if(data_counts.begin(), data_counts.end(), [&value](data_count_t<value_type> data_count) { return data_count.data == value; });
		/* or:
		auto search = std::find_if(data_counts.begin(), data_counts.end(), [&value](auto data_count) { return data_count.data == value; }); */

		if (search != data_counts.end())
			(*search).count += 1;
		else
			data_counts.insert(data_counts.end(), { value, 1 });
	}

	data_counts.shrink_to_fit();
	data_counts.data();

	printf("vector size: %zu\n", data_counts.size());
	for (auto data_count : data_counts)
		printf("%" PRIx64 ": %zu\n", data_count.data, data_count.count);
}

// bitly huffman compression

c_string_t bitly_huffman_ext(".bh");

void bitly_huffman_compress(const char *file_name_pntr)
{
	c_string_t file_name(file_name_pntr);
	m_string_t archive_name(&file_name, &bitly_huffman_ext);
}

// main programs

int call_for_each(int argc, const char **argv, void(*func)(const char *argv))
{
	if (argc == 1)
	{
		printf("usage: %s [FILENAME]...\n", argv[0]);
		return 0;
	}
	func(argv[1]);
	for (int index = 2; index < argc; index += 1)
	{
		print_line(0, '=');
		func(argv[index]);
	}
	return 0;
}

/* total functions: offsets_check, search_offsets */

template <class value_type>
void print_vec(const std::vector<value_type> vec)
{
	if (vec.size() != 0)
	{
		std::cout << vec[0];
		for (size_t index = 1; index < vec.size(); index += 1)
			std::cout << ' ' << vec[index];
	}
	std::cout << std::endl;
}

int main(int argc, const char **argv)
{
	return call_for_each(argc, argv, count_bit_width);
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
