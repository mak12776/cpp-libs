#pragma once

#include <string>

namespace cface
{
	class c_type
	{
		
	};

	class c_struct
	{
	public:
		const std::string name;
	};

	class c_function
	{
	public:
		const c_type return_type;

		const std::string name;

		const bool is_inline;
		const bool is_static;


	};
}