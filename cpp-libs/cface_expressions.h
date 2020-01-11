#pragma once

namespace cface
{
	class c_expression
	{ };

	class c_binary_operation : public c_expression
	{ };

	enum class c_statement_type
	{
		MACRO_INCLUDE, MACRO_DEFINE,
		MACRO_IF, MACRO_IFDEF, MACRO_IFNDEF,
		MACRO_ELSE, MACRO_ELIF, MACRO_ENDIF,

		IF, FOR, WHILE,
	};
}