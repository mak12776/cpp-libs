#pragma once

#include "../scl/all.h"
#include "../clib.h"

namespace bh
{
	using namespace scl;

	template <size_t size>
	using get_data_type =
		std::conditional_t<(size <= 8), uint8_t,
		std::conditional_t<(size <= 16), uint16_t,
		std::conditional_t<(size <= 32), uint32_t, uint64_t>>>;

#pragma pack(push, 1)
	template <typename data_type>
	struct data_count_t
	{
		// members
		
		data_type data;
		size_t count;

		// functions

		inline int compare(const void * a, const void * b)
		{
			const data_count_t<data_type> *pntr_a =
				static_cast<const data_count_t<data_type> *>(a);
			const data_count_t<data_type> *pntr_b =
				static_cast<const data_count_t<data_type> *>(b);

			if (pntr_a->count < pntr_b->count) return -1;
			if (pntr_a->count > pntr_b->count) return 1;
			return 0;

		}
	};

	template <typename data_type>
	struct remaining_t
	{
		// members

		data_type value;
		ubyte size;
	};

	template <size_t size, typename data_type = get_data_type<size>>
	struct count_t
	{
		// members

		std::vector<data_count_t<data_type>> data_counts;
		remaining_t<data_type> remaining;

		// functions

		constexpr size_t get_size() { return size; }

		inline void append_data_count(data_type data)
		{
			for (size_t len = 0; len < data_counts.size(); len += 1)
			{
				if (data_counts[len].data == data)
				{
					data_counts[len].count += 1;
					return;
				}
			}
			data_counts.push_back(data_count_t<data_type>{data, 1});
		}
	};
#pragma pack(pop)

	// main functions

	static inline constexpr size_t safe_eight_time(size_t size)
	{
		constexpr size_t mask = 0xE0 << (size_bytes - 1);
		if (size & mask)
		{
			err::set(err::INT_OVERFLOW);
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}
		return size << 3;
	}

	template <size_t size, typename data_type = get_data_type<size>>
	void count_size(ubuffer_t &buffer, count_t<size> &result)
	{
		if constexpr (size == (sizeof(data_type) * 8))
		{
			data_type *pntr = (data_type *)buffer.pntr;
			data_type *end = pntr + (buffer.size / sizeof(data_type));

			while (pntr != end)
				result.append_data_count(*(pntr++));

			result.remaining.size = buffer.size % sizeof(data_type);
			if (result.remaining.size)
				memcpy(&result.remaining.value, end, result.remaining.size);
		}
		else
		{
			err::set(err::UNDEFINED_BEHAVIOR);
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
		}
	}

	c_string_t sep("\\");

	static inline void count_file_name(c_string_t &parent_folder, c_string_t &name)
	{
		m_string_t path;
		ubuffer_t buffer;
		size_t buffer_bits;
		count_t<64> count;

		path.malloc_cat({ parent_folder, sep, name });
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		printf("file path: \"%s\"\n", path.pntr);

		io::fopen_fread_all(path.pntr, buffer);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		math::safe_mul(buffer.size, (size_t)8, buffer_bits);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;

		printf("file size: %zu (%zu bits)\n", buffer.size, buffer_bits);

		count_size(buffer, count);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;


	}

	static inline int main(int argc, const char **argv)
	{
		const char *main_name = (argc != 0) ? argv[0] : "main.exe";

		if (argc != 2)
		{
			printf("usage: %s [FILENAME]\n", main_name);
			return 0;
		}
	}

}