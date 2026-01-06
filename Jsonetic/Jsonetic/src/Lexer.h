#pragma once

#include <istream>
#include <vector>

namespace jsonc
{
	enum class TokenType
	{
		NONE = 0,
		CURLY_OPEN,
		CURLY_CLOSE,
		COLON,
		STRING,
		NUMBER,
		ARRAY_OPEN,
		ARRAY_CLOSE,
		COMMA,
		BOOLEAN,
		NULL_TYPE,
		END_OF_FILE
	};

	struct Token
	{
		TokenType Type;

		std::string StringData = "";
		double		NumberData = 0.0;
		bool		BooleanData = false;

		Token(TokenType type);
		Token(TokenType type, const std::string& str);
		Token(TokenType type, double number);
		Token(TokenType type, bool boolean);
	};

	class Lexer
	{
	public:
		Lexer(std::istream& stream);

		std::vector<Token> GetTokens();
		bool HasError();

	private:
		void SkipWhitespaces();
		void CollectTokens();

		Token NextToken();

		std::string CollectString();
		double		CollectNumber();
		bool		CollectBoolean();

	private:
		std::istream&		m_Stream;
		std::vector<Token>	m_Tokens;
	};
}