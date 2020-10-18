#pragma once

#include <cstdio>

#include "../scl/all.h"

namespace counter
{
	using namespace scl;

	template <typename byte_t>
	struct buffer_reader_t
	{
		virtual bool read_first_buffer(byte_t *pntr, size_t size) = 0;
		virtual bool read_buffer(byte_t *pntr, size_t size) = 0;
		virtual bool read_last_buffer(byte_t *pntr, size_t size) = 0;
	};

	inline constexpr bool is_lower(ubyte ch)
	{
		return (('a' <= ch) && (ch <= 'z'));
	}

	inline constexpr bool is_upper(ubyte ch)
	{
		return (('A' <= ch) && (ch <= 'Z'));
	}

	inline constexpr bool is_alpha(ubyte ch)
	{
		return is_lower(ch) || is_upper(ch);
	}

	inline constexpr bool is_digit(ubyte ch)
	{
		return ('0' <= ch) && (ch <= '9');
	}

	inline constexpr bool is_printable(ubyte ch)
	{
		return (' ' <= ch) && (ch <= '~');
	}

	inline constexpr bool is_alpha_digit(ubyte ch)
	{
		return is_alpha(ch) || is_digit(ch);
	}

	inline constexpr bool is_letter(ubyte ch)
	{
		return isalpha(ch) || (ch == '_');
	}

	inline constexpr bool is_symbol(ubyte ch)
	{
		return ('!' <= ch) && (ch <= '~')
			& (!is_alpha_digit(ch));
	}

	struct count_t : buffer_reader_t<ubyte>
	{
		size_t chars;
		size_t lines;
		size_t numbers;
		size_t words;

		virtual bool read_first_buffer(ubyte *pntr, size_t size)
		{
			size_t index = 0;
			while (index < size)
			{
				if (is_letter(pntr[index]))
				{
					this->chars += 1;
					this->words += 1;

					index += 1;
					while (index < size)
					{
						
					}
				}
				else if (isdigit(pntr[index]))
				{
					
				}
				else if (is_symbol(pntr[index]))
				{

				}
			}
		}
	};

	// there is two types of file readers.
	// buffered readers & full buffered readers

#if 0
	template <size_t buffer_size = 4096>
	static inline void count_file(FILE *file, count_t &count)
	{
		ubuffer_t buffer;

		io::fread_all(file, buffer);

		if (err::clear_if(err::NO_MEMORY))
		{
			buffer.pntr = mem::safe_malloc(buffer_size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			cleaner::add_free(buffer.pntr);

			// more codes here
		}
		else if (err::check())
		{
			err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			return;
		}
	}
#endif
}