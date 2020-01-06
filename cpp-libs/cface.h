#pragma once

#include <string>
#include <vector>

namespace cface
{
	class c_type
	{
		std::string type;
	};

	class c_variable
	{
		c_type type;

	};

	class c_struct
	{
		std::string name;
		std::vector<std::string> members;
	};

	class c_function
	{
		c_type return_type;

		std::string name;

		bool is_inline;
		bool is_static;


	};
}