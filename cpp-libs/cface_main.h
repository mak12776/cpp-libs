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
}