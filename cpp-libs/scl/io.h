#pragma once

namespace scl
{
	namespace low_io
	{
		// system level io functions

		static inline void safe_stat32(const char *pntr, struct _stat32 *stat_p)
		{
			if (_stat32(pntr, stat_p))
			{
				err::set(err::STAT);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		static inline void safe_stat64(const char *pntr, struct _stat64 *stat_p)
		{
			if (_stat64(pntr, stat_p))
			{
				err::set(err::STAT);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		static inline int safe_open(const char *name, int flags, int mode = 0)
		{
			int fd = _open(name, flags, mode);
			if (fd == -1)
			{
				err::set(err::OPEN);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
			return fd;
		}

		
	}

	namespace io
	{
		// safe file operations functions

		static inline FILE *safe_fopen(const char *name, const char *mode)
		{
			FILE *file = std::fopen(name, mode);

			if (file == nullptr)
			{
				err::set(err::FOPEN);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}

			return file;
		}

		static inline long safe_ftell(FILE *stream)
		{
			long value;

			value = std::ftell(stream);
			if (value == -1)
			{
				err::set(err::FTELL);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}

			return value;
		}

		static inline void safe_fseek(FILE *stream, long offset, int origin)
		{
			if (fseek(stream, offset, origin))
			{
				err::set(err::FSEEK);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}
		}

		static inline long get_file_size_long(FILE *stream)
		{
			long size;

			safe_fseek(stream, 0, SEEK_END);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			size = safe_ftell(stream);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			safe_fseek(stream, 0, SEEK_SET);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			return size;
		}

		static inline size_t get_file_size(FILE *stream)
		{
#if ULONG_MAX > SIZE_MAX
#error unsigned long is too big.
#endif
			size_t file_size = (size_t)get_file_size_long(stream);

			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			return file_size;
		}

		static inline size_t get_file_name_size(const char *name)
		{
#if SIZE_MAX == UINT64_MAX
			struct __stat64 file_stat;
			low_io::safe_stat64(name, &file_stat);
#elif SIZE_MAX == UINT32_MAX
			struct _stat32 file_stat;
			low_io::safe_stat32(name, &file_stat);
#else
#error unknown SIZE_MAX.
#endif
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			return file_stat.st_size;
		}

		// safe fread
		static inline size_t safe_fread(void *pntr, size_t size, FILE *stream)
		{
			size_t read_number;

			read_number = fread(pntr, 1, size, stream);
			if (read_number != size)
			{
				err::set(err::FREAD);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}

			return read_number;
		}

		template <typename data_type>
		static inline size_t safe_fread_data(data_type &data, FILE *file)
		{
			return safe_fread(&data, sizeof(data_type), file);
		}
		
		// safe fwrite

		static inline size_t safe_fwrite(void *pntr, size_t size, FILE *stream)
		{
			size_t write_number;

			write_number = fwrite(pntr, 1, size, stream);
			if (write_number != size)
			{
				err::set(err::FWRITE);
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
			}

			return write_number;
		}

		template<typename data_type>
		static inline size_t safe_fwrite_data(data_type data, FILE *stream)
		{
			return safe_fwrite(&data, sizeof(data_type), stream);
		}

		// other functions

		static inline void fread_all(FILE *file, void **pntr, size_t &size)
		{
			size_t file_size;

			file_size = get_file_size(file);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			(*pntr) = mem::safe_malloc(file_size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			size = safe_fread(*pntr, file_size, file);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				free(*pntr);
				return;
			}
		}

		static inline void fopen_fread_all(const char *name, void **pntr, 
			size_t &size)
		{
			FILE *file = safe_fopen(name, "rb");
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			fread_all(file, pntr, size);
			if (err::check())
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

			std::fclose(file);
		}

		static inline size_t fopen_fwrite_all(const char *name, void *pntr, size_t size)
		{
			FILE *file = safe_fopen(name, "wb");
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return 0;
			}

			size_t write_number = safe_fwrite(pntr, size, file);
			if (err::check())
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

			std::fclose(file);
			return write_number;
		}

		// buffered byte reader

#ifdef SCL_EXPERIMENTAL
		template <typename byte_t>
		struct byte_reader_t
		{
			virtual void get() = 0;
		};

		template <typename byte_t, size_t buffer_size>
		static inline void fread_byte_reader(FILE *file, byte_reader_t<byte_t> &byte_reader)
		{
			byte_t *pntr;
			size_t file_size;
			cleaner::cleaner_t<1> cleaner;

			file_size = get_file_size(file);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			pntr = (byte_t *)mem::safe_malloc(buffer_size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			cleaner.add_free(pntr);

			while (file_size >= buffer_size)
			{
				safe_fread(pntr, buffer_size, file);
				if (err::check())
				{
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
					cleaner.finish();
					return;
				}
				file_size -= buffer_size;


			}
		}
#endif

		// buffered reader

		template <typename byte_t>
		struct buffer_reader_t
		{
			virtual bool read_buffer(byte_t *pntr, size_t size) = 0;
			virtual void finish() = 0;
		};

		template <typename byte_t, size_t buffer_size = 8192>
		static inline void fread_buffered(FILE *file, buffer_reader_t<byte_t> &buffer_reader)
		{
			byte_t *pntr;
			size_t file_size;
			cleaner::cleaner_t<1> cleaner;

			file_size = get_file_size(file);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			pntr = (byte_t *)mem::safe_malloc(buffer_size);
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			cleaner.add_free(pntr);

			while (file_size >= buffer_size)
			{
				safe_fread(pntr, buffer_size, file);
				if (err::check())
				{
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
					cleaner.finish();
					return;
				}
				file_size -= buffer_size;

				if (!buffer_reader.read_buffer(pntr, buffer_size))
				{
					cleaner.finish();
					return;
				}
			}

			if (file_size)
			{
				safe_fread(pntr, file_size, file);
				if (err::check())
				{
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
					cleaner.finish();
					return;
				}

				if (!buffer_reader.read_buffer(pntr, file_size))
					buffer_reader.finish();
			}
			cleaner.finish();
		}

		template <typename byte_t, size_t size = 8192>
		static inline void fopen_fread_buffered(const char *file_name, buffer_reader_t<byte_t> &buffer_reader)
		{
			FILE *file = safe_fopen(file_name, "rb");
			if (err::check())
			{
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			fread_buffered(file, buffer_reader);
			if (err::check())
				err::push_file_info(__FILE__, __LINE__, __FUNCTION__);

			fclose(file);
		}

		// fread, fwrite all with base_buffer_t

		template <typename byte_t>
		static inline void fread_all(FILE *file, 
			base_buffer_t<byte_t> &buffer)
		{ 
			fread_all(file, (void **)&buffer.pntr, buffer.size);
		}

		template <typename byte_t>
		static inline void fopen_fread_all(const char *file,
			base_buffer_t<byte_t> &buffer)
		{
			fopen_fread_all(file, (void **)&buffer.pntr, buffer.size);
		}

		template <typename byte_t>
		static inline void fopen_fwrite_all(const char *file,
			base_buffer_t<byte_t> &buffer)
		{
			fopen_fwrite_all(file, (void *)buffer.pntr, buffer.size);
		}

		// deprecated functions
#if 0
		static inline size_t fread_all(void *pntr, size_t size, FILE *stream)
		{
			size_t read_number;

			read_number = fread(pntr, 1, size, stream);
			while (read_number != size)
			{
				if (ferror(stream))
				{
					err::set(err::FERROR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else if (feof(stream))
				{
					err::set(err::FEOF);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else
				{
					err::set(err::UNDEFINED_BEHAVIOR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				read_number += fread((ubyte *)pntr + read_number, 1, size - read_number, stream);
			}
			return read_number;
		}

		static inline size_t fwrite_all(void *pntr, size_t size, FILE *stream)
		{
			size_t write_number;

			write_number = fwrite(pntr, 1, size, stream);
			while (write_number != size)
			{
				if (ferror(stream))
				{
					err::set(err::FERROR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}
				else
				{
					err::set(err::UNDEFINED_BEHAVIOR);
					err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				}

				write_number += fwrite((ubyte *)pntr + write_number, 1, size - write_number, stream);
			}
			return write_number;
		}
#endif
	}
}