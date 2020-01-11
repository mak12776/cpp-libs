#pragma once

#include <string>

namespace cface
{
	class c_macro_include : public c_statement
	{
	public:
		bool is_global;
		c_macro_header_name name;
	};

	class c_macro_define : public c_statement
	{
	public:
		c_name name;
		c_macro_expression value;
	};

	class c_macro_undef : public c_statement
	{
	public:
		c_name name;
	};

	class c_macro_if : public c_statement
	{
	public:
		c_expression *condition;
	};

	class c_macro_ifdef : public c_statement
	{
	public:
		c_name name;
	};

	class c_macro_ifndef : public c_statement
	{
	public:
		c_name name;
	};
}