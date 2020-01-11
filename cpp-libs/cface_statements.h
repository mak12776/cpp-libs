#pragma once

namespace cface
{
	enum class c_statement_type
	{
		MACRO_INCLUDE, MACRO_DEFINE,
		MACRO_IF, MACRO_IFDEF, MACRO_IFNDEF,
		MACRO_ELSE, MACRO_ELIF, MACRO_ENDIF,

		IF, FOR, WHILE,
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