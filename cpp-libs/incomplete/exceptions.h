#pragma once

#include <stdexcept>

namespace scl
{
	static inline std::invalid_argument *new_invalid_argument(std::string function_name, std::string argument_name)
	{
		return new std::invalid_argument(
			std::string("function:") + function_name + std::string("argument: ") + argument_name
		);
	}

	class fopen_error : std::exception
	{
	private:
		const char *_name;
		const char *_mode;

	public:
		fopen_error(const char *name, const char *mode) : exception("fopen_error")
		{
			this->_name = name;
			this->_mode = mode;
		}

		fopen_error(fopen_error &other) = delete;
		fopen_error(fopen_error &&other) = delete;

		const char *name() const
		{
			return this->_name;
		}

		const char *mode() const
		{
			return this->_mode;
		}
	};

	class ftell_error : std::exception
	{
	public:
		ftell_error() : exception("ftell_error") { }

		ftell_error(ftell_error &other) = delete;
		ftell_error(ftell_error &&other) = delete;
	};

	class fseek_error : std::exception
	{
	public:
		fseek_error() : exception("fseek_error") { }

		fseek_error(fseek_error &other) = delete;
		fseek_error(fseek_error &&other) = delete;
	};

	class fread_error : std::exception
	{
	private:
		FILE *_stream;
		size_t _size;

	public:
		fread_error(FILE *stream, size_t size) : exception("fread_error") 
		{ 
			this->_stream = stream;
			this->_size = size;
		}

		fread_error(fread_error &other) = delete;
		fread_error(fread_error &&other) = delete;

		inline FILE *stream() const
		{
			this->_stream;
		}
		
		inline size_t size() const
		{
			this->_size;
		}
	};

	class fwrite_error : std::exception
	{
	private:
		FILE *_stream;
		size_t _size;

	public:
		fwrite_error(FILE *stream, size_t size) : exception("fwrite_error")
		{
			this->_stream = stream;
			this->_size = size;
		}

		fwrite_error(fwrite_error &other) = delete;
		fwrite_error(fwrite_error &&other) = delete;

		inline FILE *stream() const
		{
			this->_stream;
		}

		inline size_t size() const
		{
			this->_size;
		}
	};
}