#pragma once


namespace cface
{
	enum class c_key : uint8_t
	{
		START_INDEX,

		MACRO_PREFIX, MACRO_INCLUDE, MACRO_DEFINE, MACRO_UNDEF,
		MACRO_IF, MACRO_IFDEF, MACRO_IFNDEF,
		MACRO_ELIF, MACRO_ELSE, MACRO_ENDIF,

		STRUCT, UNION,
		IF, ELSE, FOR, WHILE,
		SWITCH, CASE, DEFAULT,

		ADD, SUB, MUL, DIV, MOD,
		AND, OR, NOT,
		EQ, NE, LT, LE, GT, GE,
		BIT_AND, BIT_OR, BIT_XOR, BIT_NOT,
		SHIFT_LEFT, SHIFT_RIGHT,

		ASSIGN,
		ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN,
		BIT_AND_ASSIGN, BIT_OR_ASSIGN, BIT_XOR_ASSIGN,
		SHIFT_LEFT_ASSIGN, SHIFT_RIGHT_ASSIGN,

		END_INDEX,
	};

	std::map<c_key, std::string> c_keywords = {
		{c_key::MACRO_PREFIX, "#"}, {c_key::MACRO_INCLUDE, "include"}, {c_key::MACRO_DEFINE, "define"}, {c_key::MACRO_UNDEF, "undef"},
		{c_key::MACRO_IF, "if"}, {c_key::MACRO_IFDEF, "ifdef"}, {c_key::MACRO_IFNDEF, "ifndef"},
		{c_key::MACRO_ELIF, "elif"}, {c_key::MACRO_ELSE, "else"},

		{c_key::STRUCT, "struct"}, {c_key::UNION, "union"},
		{c_key::IF, "if"}, {c_key::ELSE, "else"}, {c_key::FOR, "for"}, {c_key::WHILE, "while"},
		{c_key::SWITCH, "switch"}, {c_key::CASE, "case"}, {c_key::DEFAULT, "default"},

		{c_key::ADD, "+"}, {c_key::SUB, "-"}, {c_key::MUL, "*"}, {c_key::DIV, "/"}, {c_key::MOD, "%"},
		{c_key::AND, "&&"}, {c_key::OR, "||"}, {c_key::NOT, "!"},
		{c_key::EQ, "=="}, {c_key::NE, "!="}, {c_key::GT, ">"}, {c_key::GE, ">="}, {c_key::LT, "<"}, {c_key::LE, "<="},
		{c_key::BIT_AND, "&"}, {c_key::BIT_OR, "|"}, {c_key::BIT_XOR, "^"}, {c_key::BIT_NOT, "~"},
		{c_key::SHIFT_LEFT, "<<"}, {c_key::SHIFT_RIGHT, ">>"},

		{c_key::ASSIGN, "="},
		{c_key::ADD_ASSIGN, "+="}, {c_key::SUB_ASSIGN, "-="}, {c_key::MUL_ASSIGN, "*="}, {c_key::DIV_ASSIGN, "/="}, {c_key::MOD_ASSIGN, "%="},
		{c_key::BIT_AND_ASSIGN, "&="}, {c_key::BIT_OR_ASSIGN, "|="}, {c_key::BIT_XOR_ASSIGN, "^="},
		{c_key::SHIFT_LEFT_ASSIGN, "<<="}, {c_key::SHIFT_RIGHT_ASSIGN, "<<="},
	};

	inline const char *to_string(const c_key key)
	{
		switch (key)
		{
#define CASE(NAME) case NAME: return #NAME
			CASE(c_key::MACRO_PREFIX); CASE(c_key::MACRO_INCLUDE); CASE(c_key::MACRO_DEFINE); CASE(c_key::MACRO_UNDEF);
			CASE(c_key::MACRO_IF); CASE(c_key::MACRO_IFDEF); CASE(c_key::MACRO_IFNDEF);
			CASE(c_key::MACRO_ELIF); CASE(c_key::MACRO_ENDIF);

			CASE(c_key::STRUCT); CASE(c_key::UNION);
			CASE(c_key::IF); CASE(c_key::ELSE); CASE(c_key::FOR); CASE(c_key::WHILE);
			CASE(c_key::SWITCH); CASE(c_key::CASE); CASE(c_key::DEFAULT);

			CASE(c_key::ADD);
#undef CASE
		default: return "[unknown c_key]";
		}
	}

	inline std::ostream &operator<<(std::ostream &stream, const c_key &key)
	{
		const char *string = to_string(key);
		stream.write(string, std::strlen(string));
		return stream;
	}


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

	void check_keywords(std::ostream &stream = std::cout)
	{
		const uint8_t start = static_cast<uint8_t>(c_key::START_INDEX) + 1;
		const uint8_t end = static_cast<uint8_t>(c_key::END_INDEX);
		std::vector<c_key> not_exist(0);
		not_exist.reserve(c_keywords.size());

		for (uint8_t index = start; index < end; index += 1)
		{
			c_key key = static_cast<c_key>(index);
			if (c_keywords.find(key) == c_keywords.end())
				not_exist.push_back(key);
		}
		if (not_exist.size() != 0)
		{
			stream << "not exist:" << std::endl;
			for (c_key key : not_exist)
				stream << key << std::endl;
		}
	}
}