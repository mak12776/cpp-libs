#pragma once

namespace cface
{
	class c_name
	{
	public:
		std::string name;

		c_name(std::string name) : name(name) { }
	};

	class c_type
	{
	public:
		std::string type;

		c_type(std::string type) : type(type) { }
	};

	enum class c_statement_type
	{
		MACRO_INCLUDE, MACRO_DEFINE,
		MACRO_IF, MACRO_IFDEF, MACRO_IFNDEF,
		MACRO_ELSE, MACRO_ELIF, MACRO_ENDIF,

		EXPRESSION,
		IF, FOR, WHILE, FUNCTION,
	};

	class c_statement
	{
	private:
		c_statement_type type;
	protected:
		c_statement(c_statement_type type) : type(type)
		{ }
	};

	enum class c_expression_type : uint8_t
	{
		BINARY, UNARY,
	};

	class c_expression
	{ 
	private:
		c_expression_type type;
	protected:
		c_expression(c_expression_type type): type(type)
		{ }
	};
}
