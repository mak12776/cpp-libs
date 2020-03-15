#pragma once

#include <cstdint>
#include <vector>
#include <iostream>

#include "dynamic_array.h"
#include "static_dyanamic_array.h"
#include "fixed_vector.h"

namespace scl
{
	class machine
	{
#if SIZE_MAX == UINT32_MAX
#define MACHINE_32BIT 1
#elif SIZE_MAX == UINT64_MAX
#define MACHINE_64BIT 1
#else
#error unkown SIZE_MAX
#endif

	public:
		typedef std::size_t size_type;

		typedef static_dynamic_array<uint8_t> buffer_type;

		typedef dynamic_array<buffer_type *> buffers_type;
		typedef dynamic_array<size_type> pointers_type;

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

			const char *what() const override
			{
				return this->message.c_str();
			}
		};

	protected:
		buffers_type buffers;
		pointers_type pointers;

		size_type ip;
		size_type bip;

		buffer_type *inst_buffer;

	public:
		machine(size_type buffers_number, size_type pointers_number, size_type base_inst_pointer, size_type inst_pointer)
			: buffers(buffers_number, nullptr), pointers(pointers_number, 0), bip(base_inst_pointer), ip(inst_pointer)
		{ }

		enum class inst_t: uint8_t
		{
			NOOP,

			// COPY IM PI

			COPY_IM8_PI8,
			COPY_IM16_PI8,
			COPY_IM32_PI8,
#ifdef MACHINE_64BIT
			COPY_IM64_PI8,
#endif

			COPY_IM8_PI16,
			COPY_IM16_PI16,
			COPY_IM32_PI16,
#ifdef MACHINE_64BIT
			COPY_IM64_PI16,
#endif

			COPY_IM8_PI32,
			COPY_IM16_PI32,
			COPY_IM32_PI32,
#ifdef MACHINE_64BIT
			COPY_IM64_PI32,
#endif

#ifdef MACHINE_64BIT
			COPY_IM8_PI64,
			COPY_IM16_PI64,
			COPY_IM32_PI64,
			COPY_IM64_PI64,
#endif

			// COPY PI PI

			COPY_PI8_PI8,
			COPY_PI16_PI8,
			COPY_PI32_PI8,
#ifdef MACHINE_64BIT
			COPY_PI64_PI8,
#endif

			COPY_PI8_PI16,
			COPY_PI16_PI16,
			COPY_PI32_PI16,
#ifdef MACHINE_64BIT
			COPY_PI64_PI16,
#endif

			COPY_PI8_PI32,
			COPY_PI16_PI32,
			COPY_PI32_PI32,
#ifdef MACHINE_64BIT
			COPY_PI64_PI32,
#endif

#ifdef MACHINE_64BIT
			COPY_PI8_PI64,
			COPY_PI16_PI64,
			COPY_PI32_PI64,
			COPY_PI64_PI64,
#endif

			// COPY IM [PI]

			COPY_IM8_PI8A,
#if 0
			COPY_IM8_PI8A,
			COPY_IM8_PI8A,
			COPY_IM8_PI8A,
#endif

			// COPY IM [IM]

			// COPY [PI] [PI]

			// JUMP REL IM
			// JUMP DIR IM
		};


	protected:
		template <typename data_type>
		inline data_type read(const error_type &type, const std::string &name)
		{
			if (ip >= inst_buffer->size())
			{
				throw new runtime_error(
					type,
					"instruction pointer is out of range for " + name + ": " + std::to_string(ip)
				);
			}

			if (inst_buffer->size() - ip > sizeof(data_type))
			{
				throw new runtime_error(
					type,
					"insufficient buffer for " + name + ": " + std::to_string(ip)
				);
			}

			data_type value = *((data_type *)(this->inst_buffer->data()[ip]));
			ip += sizeof(data_type);
			return value;
		}

	public:
		inline void run()
		{
			inst_t inst;

			std::cout << "after\n";

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

			while (true)
			{
				inst = read<inst_t>(error_type::INVALID_INSTRUCTION_POINTER, "next instruction");

				switch (inst)
				{
				case inst_t::NOOP:
					break;

				}
			}
		}
	};
}
