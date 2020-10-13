#pragma once

namespace cface
{
	enum class c_key : uint8_t
	{
		START_INDEX,

		MACRO_PREFIX, MACRO_INCLUDE, MACRO_DEFINE, MACRO_UNDEF,
		MACRO_IF, MACRO_IFDEF, MACRO_IFNDEF,
		MACRO_ELIF, MACRO_ELSE, MACRO_ENDIF,

		AUTO, EXTERN, STATIC, REGISTER, CONST,

		STRUCT, UNION, ENUM,
		IF, ELSE, FOR, WHILE,
		SWITCH, CASE, DEFAULT,
		BREAK, CONTINUE,

		AND, OR, NOT,
		EQ, NE, LT, LE, GT, GE,

		ADD, SUB, MUL, DIV, MOD,
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
		{c_key::MACRO_ELIF, "elif"}, {c_key::MACRO_ELSE, "else"}, {c_key::MACRO_ENDIF, "endif"},

		{c_key::STRUCT, "struct"}, {c_key::UNION, "union"}, {c_key::ENUM, "enum"},
		{c_key::IF, "if"}, {c_key::ELSE, "else"}, {c_key::FOR, "for"}, {c_key::WHILE, "while"},
		{c_key::SWITCH, "switch"}, {c_key::CASE, "case"}, {c_key::DEFAULT, "default"},
		{c_key::BREAK, "break"}, {c_key::CONTINUE, "continue"},

		{c_key::AND, "&&"}, {c_key::OR, "||"}, {c_key::NOT, "!"},
		{c_key::EQ, "=="}, {c_key::NE, "!="}, {c_key::GT, ">"}, {c_key::GE, ">="}, {c_key::LT, "<"}, {c_key::LE, "<="},

		{c_key::ADD, "+"}, {c_key::SUB, "-"}, {c_key::MUL, "*"}, {c_key::DIV, "/"}, {c_key::MOD, "%"},
		{c_key::BIT_AND, "&"}, {c_key::BIT_OR, "|"}, {c_key::BIT_XOR, "^"}, {c_key::BIT_NOT, "~"},
		{c_key::SHIFT_LEFT, "<<"}, {c_key::SHIFT_RIGHT, ">>"},

		{c_key::ASSIGN, "="},
		{c_key::ADD_ASSIGN, "+="}, {c_key::SUB_ASSIGN, "-="}, {c_key::MUL_ASSIGN, "*="}, {c_key::DIV_ASSIGN, "/="}, {c_key::MOD_ASSIGN, "%="},
		{c_key::BIT_AND_ASSIGN, "&="}, {c_key::BIT_OR_ASSIGN, "|="}, {c_key::BIT_XOR_ASSIGN, "^="},
		{c_key::SHIFT_LEFT_ASSIGN, "<<="}, {c_key::SHIFT_RIGHT_ASSIGN, ">>="},
	};

	inline const char *to_string(const c_key key)
	{
		switch (key)
		{
#define CASE(NAME) case NAME: return #NAME
			CASE(c_key::START_INDEX);

			CASE(c_key::MACRO_PREFIX); CASE(c_key::MACRO_INCLUDE); CASE(c_key::MACRO_DEFINE); CASE(c_key::MACRO_UNDEF);
			CASE(c_key::MACRO_IF); CASE(c_key::MACRO_IFDEF); CASE(c_key::MACRO_IFNDEF);
			CASE(c_key::MACRO_ELIF); CASE(c_key::MACRO_ELSE); CASE(c_key::MACRO_ENDIF);

			CASE(c_key::AUTO); CASE(c_key::EXTERN); CASE(c_key::STATIC); CASE(c_key::REGISTER); CASE(c_key::CONST);

			CASE(c_key::STRUCT); CASE(c_key::UNION); CASE(c_key::ENUM);
			CASE(c_key::IF); CASE(c_key::ELSE); CASE(c_key::FOR); CASE(c_key::WHILE);
			CASE(c_key::SWITCH); CASE(c_key::CASE); CASE(c_key::DEFAULT);
			CASE(c_key::BREAK); CASE(c_key::CONTINUE);

			CASE(c_key::AND); CASE(c_key::OR); CASE(c_key::NOT);
			CASE(c_key::EQ); CASE(c_key::NE); CASE(c_key::LT); CASE(c_key::LE); CASE(c_key::GT); CASE(c_key::GE);

			CASE(c_key::ADD); CASE(c_key::SUB); CASE(c_key::MUL); CASE(c_key::DIV); CASE(c_key::MOD);
			CASE(c_key::BIT_AND); CASE(c_key::BIT_OR); CASE(c_key::BIT_XOR); CASE(c_key::BIT_NOT);
			CASE(c_key::SHIFT_LEFT); CASE(c_key::SHIFT_RIGHT);

			CASE(c_key::ASSIGN);
			CASE(c_key::ADD_ASSIGN); CASE(c_key::SUB_ASSIGN); CASE(c_key::MUL_ASSIGN); CASE(c_key::DIV_ASSIGN); CASE(c_key::MOD_ASSIGN);
			CASE(c_key::BIT_AND_ASSIGN); CASE(c_key::BIT_OR_ASSIGN); CASE(c_key::BIT_XOR_ASSIGN);
			CASE(c_key::SHIFT_LEFT_ASSIGN); CASE(c_key::SHIFT_RIGHT_ASSIGN);

			CASE(c_key::END_INDEX);

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

	void check_keywords(std::ostream &stream = std::cout)
	{
		const uint8_t start = static_cast<uint8_t>(c_key::START_INDEX) + 1;
		const uint8_t end = static_cast<uint8_t>(c_key::END_INDEX);
		std::vector<c_key> not_exist(0);
		not_exist.reserve(c_keywords.size());

		for (uint8_t length = start; length < end; length += 1)
		{
			c_key key = static_cast<c_key>(length);
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