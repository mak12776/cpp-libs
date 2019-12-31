#pragma once

#include <cstdint>
#include <vector>

#include "dynamic_array.h"
#include "static_dyanamic_array.h"
#include "fixed_vector.h"
#include "buffer.h"

namespace scl
{
	class buffer_machine
	{
	public:
		typedef std::size_t size_type;

		typedef static_dynamic_array<uint8_t> buffer_type;
		typedef dynamic_array<buffer_type *> buffers_type;
		typedef dynamic_array<size_type> pointers_type;

	protected:

		buffers_type buffers;
		pointers_type pointers;

		size_type ip;
		size_type bip;

		enum class error_type
		{
			INVALID_BASE_INSTRUCTION_POINTER,
			INVALID_INSTRUCTION_POINTER,
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
			buffer_type *inst_buffer;

			if (bip >= buffers.size())
			{
				throw new runtime_error(
					error_type::INVALID_BASE_INSTRUCTION_POINTER,
					"base instruction pointer is out of range: " + std::to_string(bip)
				);
			}

			inst_buffer = buffers[bip];

			if (inst_buffer == nullptr)
			{
				throw new runtime_error(
					error_type::INVALID_BASE_INSTRUCTION_POINTER,
					"null buffer pointer: " + std::to_string(bip)
				);
			}

			if (ip >= inst_buffer->size())
			{
				throw new runtime_error(
					error_type::INVALID_INSTRUCTION_POINTER,
					"instruction pointer is out of range: " + std::to_string(ip)
				);
			}
			
			if (ip + inst_size >= inst_buffer->size())
			{
				throw new runtime_error(
					error_type::INVALID_INSTRUCTION_POINTER,
					""
				);
			}

		}
	};
}
