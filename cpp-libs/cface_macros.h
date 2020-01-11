#pragma once

#include <string>

namespace cface
{
	enum class c_macro_type
	{
		C_DEFINE, C_UNDEF, C_INCLUDE
	};

	class c_macro
	{ 
	public:
		c_macro_type type; 
		c_macro(c_macro_type type): type(type) { }
	};

	class c_define : public c_macro
	{
	public:
		std::string name;
		std::string value;
	};

	class c_undef : public c_macro
	{
	public:
		std::string name;
	};

	class c_include : public c_macro
	{
	public:
		bool is_global;
		std::string name;
	};

	

}