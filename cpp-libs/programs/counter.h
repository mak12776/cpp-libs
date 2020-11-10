#pragma once

#include <cstdio>

#include "../scl/all.h"

namespace counter
{
	using namespace scl;

	inline constexpr bool is_lower(ubyte ch)
	{
		return (('a' <= ch) && (ch <= 'z'));
	}

	inline constexpr bool is_upper(ubyte ch)
	{
		return (('A' <= ch) && (ch <= 'Z'));
	}

	inline constexpr bool is_digit(ubyte ch)
	{
		return ('0' <= ch) && (ch <= '9');
	}

	inline constexpr bool is_printable(ubyte ch)
	{
		return (' ' <= ch) && (ch <= '~');
	}

	// combination functions

	inline constexpr bool is_alpha(ubyte ch)
	{
		return is_lower(ch) || is_upper(ch);
	}

	inline constexpr bool is_alpha_digit(ubyte ch)
	{
		return is_alpha(ch) || is_digit(ch);
	}

	inline constexpr bool is_letter(ubyte ch)
	{
		return is_alpha(ch) || (ch == '_');
	}

	enum class stat_t
	{
		WORD, DIGIT, CR, NORMAL
	};

	struct counter_t : io::buffer_reader_t<ubyte>
	{
		size_t chars = 0;
		size_t lines = 0;

		size_t digits = 0;
		size_t words = 0;
		size_t symbols = 0;

		stat_t stat;

		virtual bool read_buffer(ubyte *pntr, size_t size)
		{
			for (size_t index = 0; index < size; index += 1)
			{
				ubyte ch = pntr[index];

				chars += 1;
				if (is_letter(ch)) // a-z A-Z _
				{
					if (stat != stat_t::WORD)
					{
						words += 1;
						stat = stat_t::WORD;
					}
				}
				else if (is_digit(ch)) // 0-9
				{
					if (stat != stat_t::DIGIT)
					{
						digits += 1;
						stat = stat_t::DIGIT;
					}
				}
				else if (('!' <= ch) && (ch <= '~')) // symbols
				{
					symbols += 1;
					stat = stat_t::NORMAL;
				}
				else if (ch == '\r')
				{
					lines += 1;
					stat = stat_t::CR;
				}
				else if (ch == '\n')
				{
					if (stat != stat_t::CR) 
						lines += 1;
					stat = stat_t::NORMAL;
				}
				else
					stat = stat_t::NORMAL;
			}
			return true;
		}
		virtual void finish() { lines += 1; }
	};

	// there is two types of file readers.
	// buffered readers & full buffered readers

#if 0
	template <size_t buffer_size = 4096>
	static inline void count_file(FILE *file, counter_t &count)
	{
		ubuffer_t buffer;

		io::safe_fread_all(file, buffer);

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