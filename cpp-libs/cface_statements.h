#pragma once

namespace cface
{
	class c_macro_define : public c_statement
	{
	public:
		std::string name;
		std::string value;
	};

	class c_macro_undef : public c_macro
	{
	public:
		std::string name;
	};

	class c_macro_include : public c_macro
	{
	public:
		bool is_global;
		std::string name;
	};

	class c_macro_if : public c_macro
	{

	};

	class c_variable
	{
	public:
		c_type type;
		c_name name;
	};

	class c_variable_assign
	{
	public:
		c_type type;
		c_name name;
		c_expression value;
	};

	class c_struct
	{
	public:
		c_name name;
		std::vector<c_variable> members;
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