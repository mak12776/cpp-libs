#pragma once

namespace scl
{
	typedef int8_t byte;
	typedef uint8_t ubyte;

#define BYTE_MAX	INT8_MAX
#define BYTE_MIN	INT8_MIN
#define UBYTE_MAX	UINT8_MAX

	template <typename byte_type>
	struct dynamic_array_t
	{
		byte_type *pntr;
		size_t size;
	};

	typedef dynamic_array_t<void> vbuffer_t;
	typedef dynamic_array_t<ubyte> ubuffer_t;
	typedef dynamic_array_t<byte> buffer_t;

	struct c_string_t
	{
		const char *const pntr;
		const size_t len;

		c_string_t(const char *pntr) : pntr(pntr), len(strlen(pntr))
		{ }
	};

#if SIZE_MAX == UINT8_MAX
	typedef int8_t ssize_t;

	constexpr uint8_t size_bytes = 1;
	constexpr ubyte size_bits = 8;

#elif SIZE_MAX == UINT16_MAX
	typedef int16_t ssize_t;

	constexpr uint8_t size_bytes = 2;
	constexpr ubyte size_bits = 16;

#elif SIZE_MAX == UINT32_MAX
	typedef int32_t ssize_t;

	constexpr uint8_t size_bytes = 4;
	constexpr ubyte size_bits = 32;

#elif SIZE_MAX == UINT64_MAX
	typedef int64_t ssize_t;

	constexpr ubyte size_bytes = 8;
	constexpr ubyte size_bits = 64;

#else
#error unknown SIZE_MAX
#endif

#ifdef SCL_EXPERIMENTAL
	namespace io
	{
		// fread, fwrite malloc with base_buffer_t

		template <typename byte_t>
		static inline void fread_malloc(FILE *file, dynamic_array_t<byte_t> &buffer)
		{s
			fread_malloc(file, (void **)&buffer.pntr, buffer.size);
		}

		template <typename byte_t>
		static inline void fopen_fread_malloc(const char *name, dynamic_array_t<byte_t> &buffer)
		{
			fopen_fread_malloc(name, (void **)&buffer.pntr, &buffer.size);
		}

		template <typename byte_t>
		static inline void fopen_fwrite(const char *file, dynamic_array_t<byte_t> &buffer)
		{
			fopen_fwrite(file, (void *)buffer.pntr, buffer.size);
		};
	}
#endif // SCL_EXPERIMENTAL
}
