#pragma once

#include <cstdint>
#include <vector>

#include "array.h"
#include "vector.h"
#include "buffer.h"

namespace scl
{
	namespace data_machine
	{
		typedef uint8_t inst_t;

		const inst_t INST_NOOP = 0x00;

		template <typename data_type, size_t pointers_number, size_t buffer_number>
		class machine
		{
		private:
			array_vector<fixed_vector<data_type>, buffer_number> data_arrays;
			array_vector<size_t, pointers_number> data_pointers;

			size_t ip;
			size_t bip;

		public:

		};
	}

	class buffer_machine
	{
	public:
		typedef dynamic_array<dynamic_array<uint8_t> *> buffers_type;
		typedef dynamic_array<size_t> pointers_type;

	protected:

		buffers_type buffers;
		pointers_type pointers;

		size_t ip;
		size_t bip;

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
		};

	public:

		buffer_machine(size_t buffers_number, size_t pointers_number, size_t base_inst_pointer, size_t inst_pointer)
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
