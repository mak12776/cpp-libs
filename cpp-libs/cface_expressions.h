#pragma once

namespace cface
{
	/* unary operation */

	enum class c_unary_operation_type
	{
		POSITIVE, NEGATIVE,
		NOT, BIT_NOT,
		PREFIX_INC, POSTFIX_INC,
		PREFIX_DEC, POSTFIX_DEC,
	};

	class c_unary_operation : public c_expression
	{ 
	public:
		c_unary_operation_type type;
		c_expression *left;
		c_expression *right;

		c_unary_operation(c_unary_operation_type type, c_expression *left, c_expression *right)
			: c_expression(c_expression_type::BINARY), type(type), left(left), right(right)
		{ }
	};

	/* binary operation */

	enum class c_binary_operation_type
	{
		ADD, SUB, MUL, DIV, MOD,
		AND, OR, 
		BIT_AND, BIT_OR, BIT_XOR,

		ASSIGN,
		ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, MOD_ASSIGN, MOD_ASSIGN,
		BIT_AND_ASSIGN, BIT_OR_ASSIGN, BIT_XOR_ASSIGN,
	};

	class c_binary_operation : public c_expression
	{
	public:
		c_binary_operation_type type;
		c_expression *value;

		c_binary_operation(c_binary_operation_type type, c_expression *value)
			: c_expression(c_expression_type::UNARY), type(type), value(value)
		{ }
	};

	/* value */

	enum class c_value_type : uint8_t
	{
		NUMBER, REAL, CHAR, STRING,
	};

	class c_value : public c_expression
	{
	public:
		c_value_type type;
	};


}