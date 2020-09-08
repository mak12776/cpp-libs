#pragma once

#include "err.h"

namespace bh
{
	using namespace scl;

	scl::c_string_t archive_ext(".64bh");
	const char magic[] = { '\x64' };

#pragma pack(push, 1)

	template <typename data_type>
	struct data_count_t
	{
		data_type data;
		size_t count;

		static int compare(const void *a, const void *b)
		{
			const data_count_t<data_type> *pntr_a = static_cast<const data_count_t<data_type> *>(a);
			const data_count_t<data_type> *pntr_b = static_cast<const data_count_t<data_type> *>(b);

			if (pntr_a->count < pntr_b->count) return -1;
			if (pntr_a->count > pntr_b->count) return 1;
			return 0;
		}
	};

	template <typename data_type>
	struct counts_t
	{
		std::vector<data_count_t<data_type>> data_counts;
		std::vector<ubyte> remaining;

		inline void inc_data_count(data_type value)
		{
			for (size_t index = 0; index < data_counts.size(); index += 1)
			{
				if (data_counts[index].data = value)
				{
					data_counts[index].count += 1;
					return;
				}
			}
			data_counts.push_back({ value, 1 });
		}
	};

#pragma pack(pop)

	template <typename data_type>
	static inline size_t fwrite_counts(FILE *file, counts_t<data_type> &counts)
	{
		size_t total_write;

		const ubyte size_of_data_type = sizeof(data_type);

		// size of size_t
		total_write = io::safe_fwrite_data<ubyte>(size_bytes, file);
		if (err::check()) return total_write;

		// size of data_type
		total_write += io::safe_fwrite_data<ubyte>(size_of_data_type, file);
		if (err::check()) return total_write;

		// data_counts.size()
		total_write += io::safe_fwrite_data(counts.data_counts.size(), file);
		if (err::check()) return total_write;

		// remaining.size()
		total_write += io::safe_fwrite_data(counts.remaining.size(), file);
		if (err::check) return total_write;

		// data_counts.data()
		total_write += io::safe_fwrite(counts.data_counts.data(), 
			sizeof(data_type) * counts.data_counts.size(), file);
		if (err::check()) return total_write;

		// remaining.data()
		total_write += io::safe_fwrite(counts.remaining.data(), counts.remaining.size(), file);

		return total_write;
	}

	template <typename data_type>
	static inline size_t fread_counts(FILE *file, counts_t<data_type> &counts)
	{
		size_t total_read;
		
		const ubyte size_of_data_type = sizeof(data_type);

		ubyte ubyte_data;
		size_t vector_size;

		// size of size_t
		total_read = io::safe_fread_data(ubyte_data, file);
		if (err::check()) return total_read;

		if (ubyte_data != size_bytes)
		{
			err::set(err::INVALID_FILE_STRUCTURE);
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
			return total_read;
		}

		// size of data_type
		total_read += io::safe_fread_data(ubyte_data, file);
		if (err::check()) return total_read;

		if (ubyte_data != size_of_data_type)
		{
			err::set(err::INVALID_FILE_STRUCTURE);
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
			return total_read;
		}

		// data_counts.size()
		total_read += io::safe_fread_data(vector_size, file);
		if (err::check()) return total_read;
		counts.data_counts.resize(vector_size);

		// remaining.size()
		total_read += io::safe_fread_data(vector_size, file);
		counts.remaining.resize(vector_size);

		// data_counts.data()
		total_read += io::safe_fread(counts.data_counts.data(),
			sizeof(data_type) * counts.data_counts.size(), file);
		if (err::check()) return total_read;

		// remaining.data()
		total_read += io::safe_fread(counts.data_counts.data(),
			sizeof(data_type) * counts.data_counts.size(), file);
		
		return total_read;
	}

	c_string_t counts_64bit_ext(".64bit.counts");

	template <typename data_type>
	counts_t<data_type> count_bits(ubuffer_t &buffer)
	{
		data_type *pntr = (data_type *)buffer.pntr;
		data_type *end = pntr + (buffer.size / sizeof(data_type));

		counts_t<data_type> result;

		while (pntr != end)
			result.inc_data_count(*(pntr++));

		size_t remaining_size = buffer.size % sizeof(data_type);

		if (remaining_size)
		{
			result.remaining.reserve(remaining_size);

			ubyte *ub_pntr = (ubyte *)end;
			ubyte *ub_end = ub_pntr + remaining_size;

			while (ub_pntr != ub_end)
				result.remaining.push_back(*(ub_pntr++));
		}

		return result;
	}

#pragma pack(push, 1)
	struct data_64bit_count_t
	{
		uint64_t data;
		size_t count;
	};

	struct counts_64bit_t
	{
		std::vector<data_64bit_count_t> data_counts;
		std::vector<uint8_t> remaining;

		counts_64bit_t(size_t size) : data_counts(size)
		{ }

		inline void inc_data_count(uint64_t data)
		{
			for (size_t index = 0; index < data_counts.size(); index += 1)
			{
				if (data_counts[index].data = data)
				{
					data_counts[index].count += 1;
					return;
				}
			}
			data_counts.push_back({ data, 1 });
		}
	};
#pragma pack(pop)

	counts_64bit_t counts_64bit(ubuffer_t &buffer)
	{
		uint64_t *pntr = (uint64_t *)buffer.pntr;
		uint64_t *end = pntr + (buffer.size / sizeof(uint64_t));

		counts_64bit_t result(1024);

		while (pntr != end)
			result.inc_data_count(*(pntr++));

		uint8_t *pntr8 = (uint8_t *)end;
		uint8_t *end8 = pntr8 + (buffer.size % sizeof(uint64_t));

		while (pntr8 != end8)
			result.remaining.push_back(*(pntr8++));

		return result;
	}

	// data saver
	template <typename dtype>
	size_t save_data(c_string_t file_name, c_string_t file_ext, 
		size_t(*fwrite_data)(FILE *, dtype &), dtype &data)
	{
		m_string_t data_name;

		FILE *data_file;
		size_t total_write;
		clean_up::storage<2> clean_ups;

		data_name.malloc_cat({ file_name, file_ext });
		if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
			return 0;
		}
		clean_ups.add_free(data_name.pntr);

		data_file = io::safe_fopen(data_name.pntr, "wb");
		if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);

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
		clean_up::storage<2> clean_ups;

		data_name.malloc_cat({ file_name, file_ext });
		if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
			return 0;
		}
		clean_ups.add_free(data_name.pntr);

		data_file = io::safe_fopen(data_name.pntr, "rb");
		if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);

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
			err::push_file_info(__FILE__, __LINE__, __FUNCSIG__);
			return;
		}
	}

	template <typename data_type>
	counts_t<data_type> count_and_sort_bits(ubuffer_t &buffer)
	{
		counts_t<data_type> result;

		result = count_bits<data_type>(buffer);
		// count_bits will not failed.

		std::qsort(result.data_counts.data(), result.data_counts.size(),
			sizeof(data_count_t<data_type>), data_count_t<data_type>::compare);

		return result;
	}

	void compress(const char *file_name_pntr)
	{
		scl::c_string_t file_name(file_name_pntr);
		scl::m_string_t archive_name;

		ubuffer_t buffer;
		size_t file_bits;

		FILE *archive_file;
		constexpr bool open_archive_file = false;
		constexpr bool save_counts_data = false;

		printf("In The Name Of God.\n");

		io::set_default_width(20);
		io::print_line();

		// malloc for archive_name
		archive_name.malloc_cat({ file_name, archive_ext });
		if (err::check())
		{
			printf("error: can't malloc memory for archive file name.");
			return;
		}
		clean_up::add_free(archive_name.pntr);

		io::read_file_name(file_name.pntr, (void **)&buffer.pntr, &buffer.size);
		if (err::check())
		{
			printf("error: can't read '%s': %s\n", file_name.pntr, 
				err::get_string());
			printf("errno: %s\n", strerror(errno));

			clean_up::finish();
			return;
		}
		clean_up::add_free(buffer.pntr);

		math::safe_mul(buffer.size, (size_t)8, file_bits);
		if (err::check())
		{
			printf("error: can't multiply bytes number by 8.\n");

			clean_up::finish();
			return;
		}
		
		if (open_archive_file)
		{
			archive_file = io::safe_fopen(archive_name.pntr, "wb");
			if (err::check())
			{
				printf("error: can't fopen '%s' for writing.\n", archive_name.pntr);
				printf("errno: %s\n", strerror(errno));

				clean_up::finish();
				return;
			}
			clean_up::add_fclose(archive_file);
		}

		io::printf_ln("file name: %s", file_name.pntr);
		io::printf_ln("archive name: %s", archive_name.pntr);
		io::printf_ln("file size: %zu byte (%zu bit)", buffer.size, file_bits);

		io::print_line();

		typedef uint64_t dtype;

		counts_t<dtype> counts;

		load_data(file_name, counts_64bit_ext, fread_counts, counts);
		if (err::check())
		{
			printf("error: can't load data: %s\n", err::get_string());
			printf("errno: %s\n", strerror(errno));
			return;
		}

		if (save_counts_data)
		{
			printf("saving counts: ");

			// save_data(file_name, counts_64bit_ext, fwrite_counts_data<uint64_t>,
			// counts);
			if (err::check())
			{
				printf("failed\n");
				printf("error: saving bits: %s\n", err::get_string());
				printf("errno: %s\n", strerror(errno));

				clean_up::finish();
				return;
			}

			printf("done");
		}

		clean_up::finish();
	}
}