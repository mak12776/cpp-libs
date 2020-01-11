#pragma once

namespace cface
{
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
	};

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
	};
}