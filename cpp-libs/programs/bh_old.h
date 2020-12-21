#pragma once

#ifdef BH_INCLUDE_COUNT_64BIT

#pragma pack(push, 1)
struct data_64bit_count_t
{
	uint64_t data;
	size_t result;
};

struct counts_64bit_t
{
	std::vector<data_64bit_count_t> data_count_vector;
	std::vector<uint8_t> remaining;

	counts_64bit_t(size_t size) : data_count_vector(size)
	{ }

	inline void append_data_count(uint64_t data)
	{
		for (size_t len = 0; len < data_count_vector.size(); len += 1)
		{
			if (data_count_vector[len].data = data)
			{
				data_count_vector[len].result += 1;
				return;
			}
		}
		data_count_vector.push_back({ data, 1 });
	}
};
#pragma pack(pop)

counts_64bit_t counts_64bit(ubuffer_t &buffer)
{
	uint64_t *pntr = (uint64_t *)buffer.pntr;
	uint64_t *end = pntr + (buffer.size / sizeof(uint64_t));

	counts_64bit_t result(1024);

	while (pntr != end)
		result.append_data_count(*(pntr++));

	uint8_t *pntr8 = (uint8_t *)end;
	uint8_t *end8 = pntr8 + (buffer.size % sizeof(uint64_t));

	while (pntr8 != end8)
		result.remaining.push_back(*(pntr8++));

	return result;
}
#endif // #ifdef BH_INCLUDE_COUNT_64BIT

#ifdef BH_INCLUDE_OLD_DATA_COUNT
// fread, fwrite counts

template <typename data_type>
static inline size_t fwrite_counts(FILE *file, count_t<data_type> &counts)
{
	size_t total_write;

	const ubyte size_of_data_type = sizeof(data_type);

	// size of size_t
	total_write = io::fwrite_data<ubyte>(size_bytes, file);
	if (err::check()) return total_write;

	// size of data_type
	total_write += io::fwrite_data<ubyte>(size_of_data_type, file);
	if (err::check()) return total_write;

	// data_counts.size()
	total_write += io::fwrite_data(counts.data_count_vector.size(), file);
	if (err::check()) return total_write;

	// remaining.size()
	total_write += io::fwrite_data(counts.remaining.size(), file);
	if (err::check) return total_write;

	// data_counts.data()
	total_write += io::safe_fwrite(counts.data_count_vector.data(),
		sizeof(data_type) * counts.data_count_vector.size(), file);
	if (err::check()) return total_write;

	// remaining.data()
	total_write += io::safe_fwrite(counts.remaining.data(), counts.remaining.size(), file);

	return total_write;
}

template <typename data_type>
static inline size_t fread_counts(FILE *file, count_t<data_type> &counts)
{
	size_t total_read;

	const ubyte size_of_data_type = sizeof(data_type);

	ubyte ubyte_data;
	size_t vector_size;

	// size of size_t
	total_read = io::fread_data(ubyte_data, file);
	if (err::check()) return total_read;

	if (ubyte_data != size_bytes)
	{
		err::set(err::INVALID_FILE_STRUCTURE);
		err::push(__FILE__, __LINE__, __FUNCTION__);
		return total_read;
	}

	// size of data_type
	total_read += io::fread_data(ubyte_data, file);
	if (err::check()) return total_read;

	if (ubyte_data != size_of_data_type)
	{
		err::set(err::INVALID_FILE_STRUCTURE);
		err::push(__FILE__, __LINE__, __FUNCTION__);
		return total_read;
	}

	// data_counts.size()
	total_read += io::fread_data(vector_size, file);
	if (err::check()) return total_read;
	counts.data_count_vector.resize(vector_size);

	// remaining.size()
	total_read += io::fread_data(vector_size, file);
	counts.remaining.resize(vector_size);

	// data_counts.data()
	total_read += io::safe_fread(counts.data_count_vector.data(),
		sizeof(data_type) * counts.data_count_vector.size(), file);
	if (err::check()) return total_read;

	// remaining.data()
	total_read += io::safe_fread(counts.data_count_vector.data(),
		sizeof(data_type) * counts.data_count_vector.size(), file);

	return total_read;
}
#endif // BH_INCLUDE_OLD_DATA_COUNT


#ifdef SCL_EXPERIMENTAL
c_string_t archive_ext(".64bh");
const char magic[] = { '\x64' };

c_string_t counts_64bit_ext(".64bit.counts");
c_string_t log_file_ext(".log");

FILE *log_file;

// data saver
template <typename dtype>
size_t save_data(c_string_t file_name, c_string_t file_ext,
	size_t(*fwrite_data)(FILE *, dtype &), dtype &data)
{
	m_string_t data_name;

	FILE *data_file;
	size_t total_write;
	gc::cleaner_t<2> clean_ups;

	data_name.safe_allocate_cat({ file_name, file_ext });
	if (err::check())
	{
		err::push(__FILE__, __LINE__, __FUNCTION__);
		return 0;
	}
	clean_ups.add_free(data_name.pntr);

	data_file = io::safe_fopen(data_name.pntr, "wb");
	if (err::check())
	{
		err::push(__FILE__, __LINE__, __FUNCTION__);

		clean_ups.finish();
		return 0;
	}
	clean_ups.add_fclose(data_file);

	total_write = fwrite_data(data_file, data);

	clean_ups.finish();
	return total_write;
}

template <typename dtype>
size_t load_data(c_string_t file_name, c_string_t file_ext,
	size_t(*fread_data)(FILE *, dtype &), dtype &data)
{
	m_string_t data_name;
	FILE *data_file;
	size_t total_read;
	gc::cleaner_t<2> clean_ups;

	data_name.safe_allocate_cat({ file_name, file_ext });
	if (err::check())
	{
		err::push(__FILE__, __LINE__, __FUNCTION__);
		return 0;
	}
	clean_ups.add_free(data_name.pntr);

	data_file = io::safe_fopen(data_name.pntr, "rb");
	if (err::check())
	{
		err::push(__FILE__, __LINE__, __FUNCTION__);

		clean_ups.finish();
		return 0;
	}
	clean_ups.add_fclose(data_file);

	total_read = fread_data(data_file, data);

	clean_ups.finish();
	return total_read;
}

template <typename dtype>
static inline void data_saver(dtype &data, void(*process_data)(dtype &),
	size_t(*load_data)(dtype &), size_t(*save_data)(dtype &))
{
	load_data(data);
	if (err::check())
	{
		err::push(__FILE__, __LINE__, __FUNCTION__);
		return;
	}
}

template <size_t size>
count_t<size> count_and_sort_bits(ubuffer_t &buffer)
{
	count_t<size> result;
	typedef get_data_type<size> data_type;

	result = count_size<size>(buffer);
	// count_bits will not failed.

	std::qsort(result.data_count_vector.data(), result.data_count_vector.size(),
		sizeof(data_count_t<data_type>), data_count_t<data_type>::compare);

	return result;
}

int call_for_each(int argc, const char **argv, void(*func_pntr)(const char *argv))
{
	if (argc == 1)
	{
		printf("usage: %s [FILENAME]...\n", argv[0]);
		return 0;
	}

	func_pntr(argv[1]);
	for (int len = 2; len < argc; len += 1)
	{
		if (err::check())
			return -1;

		io::print_separator(0, '=');
		func_pntr(argv[len]);
	}

	return 0;
}

void compress(const char *file_name_pntr)
{
	scl::c_string_t file_name(file_name_pntr);
	scl::m_string_t archive_name;
	scl::m_string_t log_file_name;

	ubuffer_t buffer;
	size_t file_bits;

	// malloc log_file_name
	log_file_name.safe_allocate_cat({ file_name, log_file_ext });
	if (err::check())
	{
		printf("error: can't malloc memory for log file name.");

		gc::finish();
		return;
	}
	gc::add_free(log_file_name.pntr);

	// malloc archive_name
	archive_name.safe_allocate_cat({ file_name, archive_ext });
	if (err::check())
	{
		printf("error: can't malloc memory for archive file name.\n");

		gc::finish();
		return;
	}
	gc::add_free(archive_name.pntr);

	// fopen log_file
	log_file = io::safe_fopen(log_file_name.pntr, "wb");
	if (err::check())
	{
		printf("error: can't open log file '%s': %s\n", log_file_name.pntr, strerror(errno));

		gc::finish();
		return;
	}
	gc::add_fclose(log_file);

	// printf file names
	cl::printf_ln("file name: %s", file_name.pntr);
	cl::printf_ln("archive name: %s", archive_name.pntr);
	cl::printf_ln("log file name: %s", log_file_name.pntr);

	// read file_name into buffer
	io::malloc_fopen_fread_all(file_name.pntr, (void **)&buffer.pntr, buffer.size);
	if (err::check())
	{
		printf("error: can't read '%s': %s\n", file_name.pntr,
			err::string());
		printf("errno: %s\n", strerror(errno));

		gc::finish();
		return;
	}
	gc::add_free(buffer.pntr);

	// multiply bytes number by 8
	math::safe_mul(buffer.size, (size_t)8, file_bits);
	if (err::check())
	{
		printf("error: can't multiply bytes number by 8.\n");

		gc::finish();
		return;
	}

	// printf some informations. TODO: we need faster and more comfortable printf functions
	cl::printf_ln("file size: %zu byte (%zu bit)", buffer.size, file_bits);

	io::print_separator();

	// counting and sorting 64 bits
	typedef uint64_t dtype;
	count_t<64> counts;

	printf("counting & sorting %zu bits: ", sizeof(dtype) * 8);
	counts = count_and_sort_bits<64>(buffer);
	printf("done\n");

#if 0
	for (auto iter = counts.data_count_vector.cbegin(); iter < counts.data_count_vector.cend(); iter++)
		printf("%016llx: %zu\n", iter->data, iter->result);
#endif

	gc::finish();
}

int compress_main(int argc, const char **argv)
{
	return call_for_each(argc, argv, compress);
}
#endif
