#pragma once

#include <cstdint>
#include <vector>

#include "dynamic_array.h"
#include "fixed_vector.h"
#include "buffer.h"

namespace scl
{
	class buffer_machine
	{
	public:
		typedef std::size_t size_type;

		typedef dynamic_array<dynamic_array<uint8_t> *> buffers_type;
		typedef dynamic_array<size_type> pointers_type;

	protected:

		buffers_type buffers;
		pointers_type pointers;
		pointers_type base_pointers;

		size_type ip;
		size_type bip;

		enum class error_type
		{

		};
	
		class runtime_error : public std::exception
		{ 
		private:
			error_type error;
			std::string message;

		public:
			runtime_error(error_type error, std::string message)
			{
				this->error = error;
				this->message = message;
			}

			const char *what()
			{
				return this->message.c_str();
			}
		};

	public:

		buffer_machine(size_type buffers_number, size_type pointers_number, size_type base_inst_pointer, size_type inst_pointer)
			: buffers(buffers_number), pointers(pointers_number), bip(base_inst_pointer), ip(inst_pointer)
		{ }

		static constexpr uint8_t inst_size = 1;
		typedef uint8_t inst_t;

		static constexpr inst_t INST_NOOP = 0x00;

		inline void run()
		{
			if (bip >= buffers.size())
			{
				
			}
		}
	};
}
