#pragma once

namespace cface
{
	typedef std::string c_name;
	typedef std::string c_type;

	template <typename type>
	using list = std::vector<type>;

	typedef std::string c_macro_expression;
	typedef std::string c_macro_header_name;
}