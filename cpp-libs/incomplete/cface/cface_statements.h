#pragma once

namespace cface
{
	class c_struct
	{
	public:
		c_name name;
		list<c_variable> members;
	};

	class c_argument
	{
	public:
		bool is_dots;
		c_type type;
		c_name name;
	};

	class c_function
	{
	public:
		bool is_inline;
		bool is_static;

		c_type return_type;
		std::string name;
		std::vector<c_argument> arguments;
	};
}