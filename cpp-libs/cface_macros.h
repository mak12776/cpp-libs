#pragma once

#include <string>

namespace cface
{
	enum class c_macro_type
	{
		C_DEFINE, C_UNDEF, C_INCLUDE
	};

	struct c_macro
	{ 
		c_macro_type type; 
		c_macro(c_macro_type type): type(type) { }
	};

	struct c_define : public c_macro
	{
		std::string name;
		std::string value;
	};

	struct c_undef : public c_macro
	{
		std::string name;
	};

	struct c_include : public c_macro
	{
		bool is_global;
		std::string name;
	};

}