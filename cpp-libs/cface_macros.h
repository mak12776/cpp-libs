#pragma once

#include <string>

namespace cface
{
	class c_macro
	{
	protected:
		virtual ~c_macro() {};
		virtual void parent() = 0;
	};

	class c_define : public c_macro
	{
	private:
		virtual void parent() override { }

	public:
		std::string name;
		std::string value;
	};

	class c_include : public c_macro
	{
	private:
		virtual void parent() override { }

	public:
		bool is_global;
		std::string name;
	};

}