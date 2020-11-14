#pragma once

#include <inttypes.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <thread>
#include <algorithm>
#include <chrono>
#include <array>
#include <execution>
#include <windows.h>
#include <any>

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

#ifdef INCLUDE_OLD_DATA_COUNT
template <typename data_type>
struct data_count_t
{
	data_type data;
	size_t count;
};
#endif

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
		printf("index: %zu\n", len);
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

	for (size_t length = 0; length < counts_size; length += 1)
		counts[length] = 0;

	// add to counts for each byte
	for (size_t length = 0; length < buffer->size; length += 1)
		counts[buffer->pntr[length]] += 1;

	for (size_t length = 0; length < counts_size; length += 1)
		if (counts[length] == 0)
			*zero_counts_number += 1;
	printf("done\n");
}

void sorting_bytes(ubuffer_t *buffer, size_t counts[counts_size], byte_count_t byte_counts[counts_size])
{
	printf("sorting bytes: ");

	for (size_t length = 0; length < counts_size; length += 1)
	{
		byte_counts[length].value = (ubyte)length;
		byte_counts[length].count = counts[length];
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

c_string_t offsets_ext(".offsets");

// search offsets

void search_offsets(const char *_file_name)
{
	ubuffer_t buffer;
	FILE *info_file;

	c_string_t file_name(_file_name);
	m_string_t offsets_file_name;

	offsets_file_name.safe_allocate_cat({ file_name, offsets_ext });
	if (err::check()) return;

	cleaner::add_free((void *)offsets_file_name.pntr);

	// read entire file into buffer
	io::malloc_fopen_fread(file_name.pntr, (void **)&buffer.pntr, buffer.size);
	if (err::check())
	{
		printf("error: %s\n", err::string());
		if (err::default_err.num == err::FOPEN)
			printf("fopen error: %s\n", strerror(errno));
		return;
	}

	// opern info file for writing
	info_file = io::safe_fopen(offsets_file_name.pntr, "wb");
	if (err::check()) return;

	cleaner::add_fclose(info_file);

	// log info file name
	io::set_default_width(20);

	printf("info file created: %s\n", offsets_file_name.pntr);
	io::print_separator();

	printf_file_info(file_name.pntr, buffer.size);
	io::print_separator();

	size_t counts[counts_size];
	size_t zero_counts_number = 0;
	byte_count_t byte_counts[counts_size];
	size_t maximum_count;

	counting_bytes(&buffer, counts, &zero_counts_number);
	sorting_bytes(&buffer, counts, byte_counts);

	io::print_separator();

	// log some informations
	maximum_count = byte_counts[counts_size - 1].count;

	printf("zero counts number: %zu\n", zero_counts_number);
	printf("maximum count: %zu (%.1f%%)\n", maximum_count, ((double)maximum_count / (double)buffer.size) * 100);

	io::print_separator();

	// breaking the file:
	// printf("breaking up the file:");

	size_t *offsets = mem::safe_malloc_array<size_t>(maximum_count);
	if (err::check())
	{
		printf("error: can't allocate %zu offsets.\n", maximum_count);
		cleaner::finish();
		return;
	}
	cleaner::add_delete(offsets);

	io::safe_fwrite(counts, sizeof(counts), info_file);
	if (err::check())
	{
		printf("error: while writing counts on info file: %s\n", strerror(errno));
		cleaner::finish();
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

		io::safe_fwrite(offsets, sizeof(offsets[0]) * offsets_index, info_file);
		if (err::check())
		{
			printf("error: while writing offsets of %hhx on info file: %s\n", value, strerror(errno));
			cleaner::finish();
			return;
		}

		value += 1;
	} while (value != 0);

	io::reset_default_width();
}

// offsets check

void offsets_check(const char *_file_name)
{
	constexpr size_t counts_size = 256;
	size_t counts[counts_size];
	size_t info_counts[counts_size];

	c_string_t file_name(_file_name);
	m_string_t info_file_name;

	info_file_name.safe_allocate_cat({ file_name, offsets_ext });
	if (err::check())
	{
		printf("error: can't allocate memory for info_file_name\n");
		return;
	}

	cleaner::add_free((void *)info_file_name.pntr);

	ubuffer_t buffer;

	io::malloc_fopen_fread(file_name.pntr, (void **)&buffer.pntr, buffer.size);
	if (err::check())
	{
		printf("error: can't read input file: %s\n", file_name.pntr);
		printf("errno: %s\n", strerror(errno));
		cleaner::finish();
		return;
	}
	cleaner::add_free(buffer.pntr);

	FILE *info_file = io::safe_fopen(info_file_name.pntr, "rb");
	if (err::check())
	{
		printf("error: can't fopen: %s\n", info_file_name.pntr);
		printf("errno: %s\n", strerror(errno));
		cleaner::finish();
		return;
	}
	cleaner::add_fclose(info_file);

	for (size_t length = 0; length < counts_size; length += 1)
		counts[length] = 0;

	for (size_t length = 0; length < buffer.size; length += 1)
		counts[buffer.pntr[length]] += 1;

	io::safe_fread((void *)info_counts, sizeof(info_counts), info_file);
	if (err::check())
	{
		printf("error: can't read info file: %s\n", info_file_name.pntr);
		printf("errno: %s\n", strerror(errno));
		cleaner::finish();
		return;
	}

	if (memcmp(counts, info_counts, sizeof(counts)) != 0)
	{
		printf("offsets check: invalid counts\n");
		cleaner::finish();
		return;
	}

	size_t offsets_size = *std::max_element(info_counts, info_counts + counts_size);

	printf("max counts: %zu\n", offsets_size);

	size_t *offsets = mem::new_array<size_t>(offsets_size);
	if (err::check())
	{
		printf("error: can't allocate memory for %zu offsets\n", offsets_size);
		return;
	}
	cleaner::add_delete(offsets);

	ubyte value = 0;
	do
	{
		io::safe_fread(offsets, sizeof(offsets[0]) * counts[value], info_file);
		if (err::check())
		{
			printf("error: while reading offsets of %hhu: %s", value, strerror(errno));
			cleaner::finish();
			return;
		}


		for (size_t length = 0; length < counts[value]; length += 1)
		{
			if (offsets[length] > buffer.size)
			{
				printf("offsets check: offset for %hhu at %zu test bigger than size: %zu\n", value, length, buffer.size);
				cleaner::finish();
				return;
			}

			if (buffer.pntr[offsets[length]] != value)
			{
				printf("offsets check: offset for %hhu at %zu dosn't match ubyte at %zu on input file '%s'\n", value, length, offsets[length], file_name.pntr);
				cleaner::finish();
				return;
			}
		}

		printf("offsets check: %02hhx successfull.\n", value);

		value += 1;
	} while (value != 0);

	printf("everything successfull.\n");
	cleaner::finish();
	return;
}

#ifdef INCLUDE_OLD_DATA_COUNT
// count random-bit-width

void count_bit_width(const char *file_name)
{
	ubuffer_t buffer;
	const size_t file_name_size = strlen(file_name);

	// read input file
	io::read_file_name(file_name, (void **)&buffer.pntr, &buffer.size);
	if (err::check())
	{
		printf("error: can't read %s: %s\n", file_name, err::string());
		printf("errno: %s\n", std::strerror(errno));
		return;
	}
	clean_up::add_free(buffer.pntr);

	// count 64bits
	using value_type = uint64_t;

	std::vector<data_count_t<value_type>> data_count_vector(1024);
	size_t data_counts_index = 0;

	const size_t max_buffer_index = buffer.size / 8;
	size_t *pntr = (size_t *)buffer.pntr;

	for (size_t index = 0; index < max_buffer_index; index += 1)
	{
		value_type value = pntr[index];
		std::vector<data_count_t<value_type>>::iterator search = std::find_if(data_count_vector.begin(), data_count_vector.end(), [&value](data_count_t<value_type> data_counts) { return data_counts.data == value; });
		/* or:
		auto search = std::find_if(data_counts.begin(), data_counts.end(), [&value](auto data_counts) { return data_counts.data == value; }); */

		if (search != data_count_vector.end())
			(*search).count += 1;
		else
			data_count_vector.insert(data_count_vector.end(), { value, 1 });
	}

	data_count_vector.shrink_to_fit();

	printf("vector size: %zu\n", data_count_vector.size());
	for (auto data_counts : data_count_vector)
		printf("%" PRIx64 ": %zu\n", data_counts.data, data_counts.count);
}
#endif

/* total functions: offsets_check, search_offsets, bh::compress */

template <class value_type>
void print_vec(const std::vector<value_type> vec)
{
	if (vec.size() != 0)
	{
		std::cout << vec[0];
		for (size_t length = 1; length < vec.size(); length += 1)
			std::cout << ' ' << vec[length];
	}
	std::cout << std::endl;
}

template <typename value_type, size_t _size>
struct array_t
{
	value_type data[_size];

	constexpr size_t size() { return _size; }
};

template <typename value_type>
struct mem_array
{
	value_type *pntr;
	size_t len;
	size_t size;
};

template <typename value_type>
struct linked_array_node
{

};

template <typename value_type>
struct linked_array
{

};

#if 0
template <size_t size,
	std::enable_if_t<(size <= 8), int> = (int)0
>
struct data_count_t
{
	uint8_t data;
	size_t count;
};

template <size_t size,
	std::enable_if_t<(0 > size) && (size <= 8), int> = (int)0
		>
	struct data_count_t
{
	uint8_t data;
	size_t count;
};
#endif

/*

template <
	template<
		size_t _err_size, err::err_t<_err_size> &_err,
		size_t _cleaner_size, cleaner::cleaner_t<_cleaner_size> &_cleaner,

		mem::malloc_t &_malloc, mem::realloc_t &_realloc, mem::free_t &_free,
		mem::mem_t<_malloc, _realloc, _free, _err_size, _err> &_mem
	>
	typename ncl_t
>
void func20()
{

}
*/

struct base_mem_t
{ };

typedef void *(&malloc_t)(size_t);
typedef void *(&realloc_t)(void *, size_t);
typedef void(&free_t)(void *);

template <malloc_t _malloc, realloc_t _realloc, free_t _free>
struct mem_t : base_mem_t
{
	constexpr void *malloc(size_t size)
	{
		return _malloc(size);
	}
	constexpr void *realloc(void *pntr, size_t size)
	{
		return _realloc(pntr, size);
	}
	constexpr void free(void *pntr)
	{
		return _free(pntr);
	}
};

typedef mem_t<malloc, realloc, free> default_mem_t;
default_mem_t default_mem;

base_mem_t default_base_mem_t = default_mem;

template <base_mem_t &_base_mem>
void func()
{
	mem_t<malloc, realloc, free> _mem = _base_mem;

	_mem.malloc(100);
}


//int main(int argc, const char **argv)
//{
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
		size_t size = io::safe_get_size(name);
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

//	return 0;
//}