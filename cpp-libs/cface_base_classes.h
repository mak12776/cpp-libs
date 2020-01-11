#pragma once

namespace cface
{
	enum class c_statement_type
	{
		MACRO_INCLUDE, MACRO_DEFINE,
		MACRO_IF, MACRO_IFDEF, MACRO_IFNDEF,
		MACRO_ELSE, MACRO_ELIF, MACRO_ENDIF,
	
		FUNCTION,
		IF, FOR, WHILE, 
		EXPRESSION,
	};

	class c_statement
	{
	private:
		c_statement_type type;
	protected:
		c_statement(c_statement_type type) : type(type) { }
		c_statement(c_statement& other) = delete;
		c_statement(c_statement&& other) = delete;

	public:
		inline const c_statement_type statement_type() { return this->type; }
	};

	enum class c_expression_type : uint8_t
	{
		BINARY, UNARY,
		VALUE,
	};

	class c_expression : public c_statement
	{ 
	private:
		c_expression_type type;
	protected:
		c_expression(c_expression_type type) 
			: c_statement(c_statement_type::EXPRESSION), type(type)
		{ }
	public:
		inline const c_expression_type expression_type() { return this->type; }
	};
}
