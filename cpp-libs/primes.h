#pragma once


template <typename data_type>
struct simple_data
{
	size_t size;
	data_type *pntr;

	simple_data() : size(0), pntr(nullptr)
	{ }

	simple_data(size_t size)
	{
		using namespace scl;

		pntr = mem::safe_malloc_array<data_type>(size);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return;
		this->size = size;
	}

	inline bool read(FILE *file)
	{
		using namespace scl;

		ubyte size_of_size;
		ubyte size_of_data;
		size_t buffer_size;

		// size of size

		io::safe_fread_data<ubyte>(size_of_size, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		if (size_of_size != sizeof(size_t))
		{
			err::set(err::INVALID_FILE_STRUCTURE);
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return true;
		}

		// size of data

		io::safe_fread_data<ubyte>(size_of_data, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		if (size_of_data != sizeof(data_type))
		{
			err::set(err::INVALID_FILE_STRUCTURE);
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return true;
		}

		// size

		io::safe_fread_data(buffer_size, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		if (size != buffer_size)
		{
			err::set(err::INVALID_FILE_STRUCTURE);
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return true;
		}

		// data array

		for (size_t index = 0; index < size; index += 1)
		{
			io::safe_fread_data<data_type>(pntr[index], file);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return true;
		}

		return false;
	}

	inline bool write(FILE *file)
	{
		using namespace scl;

		ubyte size_of_size = sizeof(size_t);
		ubyte size_of_data = sizeof(data_type);

		// size of size

		io::safe_fwrite_data<ubyte>(size_of_size, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		// size of data

		io::safe_fwrite_data<ubyte>(size_of_data, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		// size

		io::safe_fwrite_data(size, file);
		if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
			return true;

		// data array

		for (size_t index = 0; index < size; index += 1)
		{
			io::safe_fwrite_data<data_type>(pntr[index], file);
			if (err::check_push_file_info(__FILE__, __LINE__, __FUNCTION__))
				return true;
		}

		return false;
	}
};

template <typename data_type>
bool find_primes(simple_data<data_type> &primes)
{
	if (primes.size == 0)
		return false;

	primes.pntr[0] = 2;

	data_type next_prime = 3;
	size_t index = 1;
	while (index < primes.size)
	{
		bool is_prime = true;
		for (size_t check_index = 0; check_index < index; check_index += 1)
		{
			if (next_prime % primes.pntr[check_index] == 0)
			{
				is_prime = false;
				break;
			}
		}

		if (is_prime)
			primes.pntr[index++] = next_prime;
		next_prime += 2;
	}

	return false;
}