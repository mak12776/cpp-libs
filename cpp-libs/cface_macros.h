#pragma once

#include <string>

namespace cface
{
	enum class c_macro_type
	{
		C_DEFINE, C_UNDEF, C_INCLUDE, 
		C_IF, C_IFDEF, C_IFNDEF,
	};

	class c_macro
	{ 
	public:
		c_macro_type type; 
		c_macro(c_macro_type type): type(type) { }
	};
}