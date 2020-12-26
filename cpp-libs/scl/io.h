#pragma once

#include <cstdio>

namespace scl
{
	namespace low_io
	{
		// system level io functions

		static inline void safe_stat32(const char *pntr, struct _stat32 *stat_p)
		{
			if (_stat32(pntr, stat_p)) 
				err::set_push(err::STAT, ERR_ARGS);
		}

		static inline void safe_stat64(const char *pntr, struct _stat64 *stat_p)
		{
			if (_stat64(pntr, stat_p))
				err::set_push(err::STAT, ERR_ARGS);
		}

		static inline int safe_open(const char *name, int flags, int mode = 0)
		{
			int fd = _open(name, flags, mode);
			if (fd == -1)
				err::set_push(err::OPEN, ERR_ARGS);
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
				err::set_push(err::FOPEN, ERR_ARGS);

			return file;
		}

		static inline long safe_ftell(FILE *stream)
		{
			long value;

			value = std::ftell(stream);
			if (value == -1)
				err::set_push(err::FTELL, ERR_ARGS);

			return value;
		}

		static inline void safe_fseek(FILE *stream, long offset, int origin)
		{
			if (fseek(stream, offset, origin))
				err::set_push(err::FSEEK, ERR_ARGS);
		}

		static inline long safe_get_size_long(FILE *stream)
		{
			long size;

			safe_fseek(stream, 0, SEEK_END); ERR_CHECK_RETURN_VALUE(0);
			size = safe_ftell(stream); ERR_CHECK_RETURN_VALUE(0);
			safe_fseek(stream, 0, SEEK_SET); ERR_CHECK_RETURN_VALUE(0);

			return size;
		}

		static inline size_t safe_get_size(FILE *stream)
		{
#if ULONG_MAX > SIZE_MAX
#error unsigned long is too big.
#endif
			size_t file_size = (size_t)safe_get_size_long(stream); ERR_CHECK_RETURN_VALUE(0);
			return file_size;
		}

		static inline size_t safe_get_size(const char *name)
		{
#if SIZE_MAX == UINT64_MAX
			struct __stat64 file_stat;
			low_io::safe_stat64(name, &file_stat); ERR_CHECK_RETURN_VALUE(0);
#elif SIZE_MAX == UINT32_MAX
			struct _stat32 file_stat;
			low_io::safe_stat32(name, &file_stat); ERR_CHECK_RETURN_VALUE(0);
#else
#error unknown SIZE_MAX.
#endif
			return file_stat.st_size;
		}

		// safe fread, fwrite


		static inline size_t safe_fread(void *pntr, size_t size, FILE *stream)
		{
			size_t read_number;

			read_number = fread(pntr, 1, size, stream);
			if (read_number != size)
				err::set_push(err::FREAD, ERR_ARGS);

			return read_number;
		}

		static inline size_t safe_fwrite(void *pntr, size_t size, FILE *stream)
		{
			size_t write_number;

			write_number = fwrite(pntr, 1, size, stream);
			if (write_number != size)
				err::set_push(err::FWRITE, ERR_ARGS);

			return write_number;
		}

		// fread, fwrite data

		template <typename data_type>
		static inline size_t fread_data(data_type &data, FILE *file)
		{
			return safe_fread(&data, sizeof(data_type), file);
		}

		template<typename data_type>
		static inline size_t fwrite_data(data_type &data, FILE *stream)
		{
			return safe_fwrite(&data, sizeof(data_type), stream);
		}

		// fread, fwrite array

		template <typename data_type>
		static inline size_t fread_array(data_type *pntr, size_t data_number, FILE *file)
		{
			size_t read_number;
			if constexpr (sizeof(data_type) != 1)
			{
				math::safe_mul(data_number, sizeof data_type, data_number);
				ERR_CHECK_RETURN_VALUE(0);
			}

			read_number = safe_fread((void *)pntr, data_number, file); ERR_CHECK_NO_RETURN;
			return read_number;
		}

		template <typename data_type>
		static inline size_t fwrite_array(data_type *pntr, size_t data_number, FILE *file)
		{
			size_t write_number;
			if constexpr (sizeof(data_type) != 1)
			{
				math::safe_mul(data_number, sizeof data_type, data_number); 
				ERR_CHECK_RETURN_VALUE(0);
			}

			write_number = safe_fwrite(pntr, data_number, file); ERR_CHECK_NO_RETURN;
			return write_number;
		}

		// fopen fread, fwrite

		static inline size_t fopen_fread(const char *name, void *pntr, size_t size)
		{
			FILE *file = safe_fopen(name, "rb"); ERR_CHECK_RETURN_VALUE(0);
			size_t read_number = safe_fread(pntr, size, file); ERR_CHECK_NO_RETURN;
			std::fclose(file);
			return read_number;
		}

		static inline size_t fopen_fwrite(const char *name, void *pntr, size_t size)
		{
			FILE *file = safe_fopen(name, "wb"); ERR_CHECK_RETURN_VALUE(0);
			size_t write_number = safe_fwrite(pntr, size, file); ERR_CHECK_NO_RETURN;
			std::fclose(file);
			return write_number;
		}

		// malloc fread

		static inline void malloc_fread(FILE *file, void **pntr, size_t size)
		{
			(*pntr) = mem::safe_malloc(size); ERR_CHECK_RETURN;
			safe_fread(*pntr, size, file); ERR_CHECK_RETURN_VALUE(free(*pntr));
		}

		static inline void malloc_fopen_fread(const char *name, void **pntr, size_t size)
		{
			FILE *file = safe_fopen(name, "rb"); ERR_CHECK_RETURN;
			malloc_fread(file, pntr, size); ERR_CHECK_NO_RETURN;
			std::fclose(file);
		}

		// malloc fread array

		template <typename data_type>
		static inline void malloc_fread_array(FILE *file, data_type **pntr, size_t size)
		{
			if constexpr (sizeof(data_type) != 1)
			{
				math::safe_mul(size, sizeof(data_type), size); 
				ERR_CHECK_RETURN;
			}

			safe_fread(*pntr, size, file); 
			ERR_CHECK_RETURN_VALUE(free(*pntr));
		}

		template <typename data_type>
		static inline void mallod_fopen_fread_array(const char *name, data_type **pntr, size_t size)
		{
			FILE *file = safe_fopen(name, "rb"); ERR_CHECK_RETURN;
			malloc_fread_array<data_type>(file, pntr, size); ERR_CHECK_NO_RETURN;
			std::fclose(file);
		}
		
		// malloc fread all

		static inline void malloc_fread_all(FILE *file, void **pntr, size_t &size)
		{
			size_t file_size;

			file_size = safe_get_size(file); ERR_CHECK_RETURN;
			(*pntr) = mem::safe_malloc(file_size); ERR_CHECK_RETURN;

			safe_fread(*pntr, file_size, file); ERR_CHECK_RETURN_VALUE(free(*pntr));
			size = file_size;
		}

		static inline void malloc_fopen_fread_all(const char *name, void **pntr, size_t &size)
		{
			FILE *file = safe_fopen(name, "rb"); ERR_CHECK_RETURN;
			malloc_fread_all(file, pntr, size); ERR_CHECK_NO_RETURN;
			std::fclose(file);
		}

		// malloc fread array all

		template <typename data_type>
		static inline void malloc_fread_array_all(FILE *file, data_type **pntr, size_t &size)
		{
			size_t file_size;
			size_t block_size;

			file_size = safe_get_size(file); ERR_CHECK_RETURN;

			block_size = file_size;
			if constexpr (sizeof(data_type) != 1)
			{
				math::safe_upper_bound(block_size, sizeof(data_type), block_size); 
				ERR_CHECK_RETURN;
			}

			(*pntr) = (data_type *)mem::safe_malloc(block_size); ERR_CHECK_RETURN;

			safe_fread(*pntr, file_size, file); ERR_CHECK_RETURN_VALUE(mem::free(*pntr));
			size = block_size / sizeof(data_type);
		}

		template <typename data_type>
		static inline void malloc_fopen_fread_array_all(const char *name, data_type **pntr, size_t &size)
		{
			FILE *file = safe_fopen(name, "rb"); ERR_CHECK_RETURN;
			malloc_fread_array_all(file, pntr, size); ERR_CHECK_NO_RETURN;
			std::fclose(file);
		}

		// buffered reader

		template <typename byte_t>
		struct buffer_reader_t
		{
			virtual bool read_buffer(byte_t *pntr, size_t size) = 0;
			virtual void finish() = 0;

			template <size_t buffer_size = 8192>
			inline void fread_all(FILE *file)
			{
				byte_t *pntr;
				size_t file_size;

				file_size = safe_get_size(file); ERR_CHECK_RETURN;
				pntr = (byte_t *)mem::safe_malloc(buffer_size); ERR_CHECK_RETURN;

				while (file_size >= buffer_size)
				{
					safe_fread(pntr, buffer_size, file); ERR_CHECK_RETURN_VALUE(mem::free(pntr));
					file_size -= buffer_size;

					if (!read_buffer(pntr, buffer_size))
						return mem::free(pntr);
				}

				if (file_size)
				{
					safe_fread(pntr, file_size, file); ERR_CHECK_RETURN_VALUE(mem::free(pntr));

					if (!read_buffer(pntr, file_size))
						return mem::free(pntr);
				}
				mem::free(pntr);
				finish();
			}

			template <size_t buffer_size = 8192>
			inline void fopen_fread_all(const char *name)
			{
				FILE *file = fopen(name, "rb"); ERR_CHECK_RETURN;
				fread_all(file); ERR_CHECK_NO_RETURN;
				std::fclose(file);
			}
		};

		// file reader, file writer

		struct file_reader_t
		{
			FILE *file;
			size_t total_read;


			inline bool read(void *buffer, size_t size)
			{
				size_t read_number = fread(buffer, 1, size, file);
				total_read += read_number;
				return read_number != size;
			}

			template <typename data_t>
			inline bool read(data_t &data)
			{
				return read(&data, sizeof data_t);
			}

			inline void safe_read(void *buffer, size_t size)
			{
				size_t read_number = fread(buffer, 1, size, file);
				if (read_number != size)
				{
					err::set(err::FREAD);
					err::push(__FILE__, __LINE__, __FUNCTION__);
				}
			}

			template <typename data_t>
			inline void safe_read(data_t &data)
			{
				safe_read(&data, sizeof data_t);
			}
		};

		struct file_writer_t
		{
			FILE *file;
			size_t total_write;

			inline bool write(void *buffer, size_t size)
			{
				size_t write_number = fwrite(buffer, 1, size, file);
				total_write += write_number;
				return write_number != size;
			}

			template <typename data_t>
			inline bool write(data_t &data)
			{
				return write(&data, sizeof data_t);
			}

			inline void safe_write(void *buffer, size_t size)
			{
				size_t write_number = fwrite(buffer, 1, size, file);
				total_write += write_number;
				if (write_number != size)
				{
					err::set(err::FWRITE);
					err::push(__FILE__, __LINE__, __FUNCTION__);
				}
			}

			template <typename data_t>
			inline void safe_write(data_t &data)
			{
				safe_write(&data, sizeof data_t);
			}
		};

		// cache file

		struct cache_t
		{
			virtual bool read(FILE *file) = 0;
			virtual bool write(FILE *file) = 0;
		};

		template <typename data_type>
		static inline bool cached_function(const char *file_name, bool(&func)(data_type &), data_type &data)
		{
			FILE * file;
			bool result;

			file = safe_fopen(file_name, "rb");
			if (err::check())
			{
				if (errno == ENOENT)
				{
					err::clear();
					if (func(data))
						return true;

					file = safe_fopen(file_name, "wb");
					if (err::check_push(__FILE__, __LINE__, __FUNCTION__))
						return true;

					result = data.write(file);
					fclose(file);
					return result;
				}

				err::push(__FILE__, __LINE__, __FUNCTION__);
				return true;
			}

			result = data.read(file);
			fclose(file);
			return result;
		}

#if 0
		// file cache

		template <typename data_type>
		struct file_cache_t
		{
			typedef bool(&loader_t)(FILE *file, data_type &);
			typedef bool(&saver_t)(FILE * file, data_type &);
			typedef bool(&func_t)(data_type &);

			const char *file_name;
			loader_t loader;
			saver_t saver;
			func_t func;

			file_cache_t(const char *name, func_t func, loader_t loader, saver_t saver)
				: file_name(name), loader(loader), saver(saver), func(func)
			{ }

			inline bool run(data_type &data)
			{
				FILE *file;

				file = safe_fopen(file_name, "rb");
				if (err::check())
				{
					if (errno == std::ENONT)
					{
						err::clear();
						if (func(data))
							return true;

						file = safe_fopen(file_name, "wb");
						if (err::check_push(__FILE__, __LINE__, __FUNCTION__))
							return true;

						if (saver(file, data))
						{
							fclose(file);
							return true;
						}

						fclose(file);
						return false;
					}

					err::push(__FILE__, __LINE__, __FUNCTION__);
					return true;
				}

				if (loader(file, data))
				{
					fclose(file);
					return true;
				}

				fclose(file);
				return false;
			}
		};
#endif
		
		// deprecated functions

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
			gc::gc_t<1> gc;

			file_size = get_file_size(file);
			if (err::check())
			{
				err::push(__FILE__, __LINE__, __FUNCTION__);
				return;
			}

			pntr = (byte_t *)mem::safe_malloc(buffer_size);
			if (err::check())
			{
				err::push(__FILE__, __LINE__, __FUNCTION__);
				return;
			}
			gc.add_free(pntr);

			while (file_size >= buffer_size)
			{
				safe_fread(pntr, buffer_size, file);
				if (err::check())
				{
					err::push(__FILE__, __LINE__, __FUNCTION__);
					gc.finish();
					return;
				}
				file_size -= buffer_size;
			}
		}
#endif
	}
}