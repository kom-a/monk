#include "Lexer.h"

#include <sstream>

namespace jsonc
{
	Token::Token(TokenType type)
		: Type(type)
	{ }

	Token::Token(TokenType type, const std::string& str)
		: Type(type), StringData(str)
	{ }

	Token::Token(TokenType type, double number)
		: Type(type), NumberData(number)
	{ }

	Token::Token(TokenType type, bool boolean)
		: Type(type), BooleanData(boolean)
	{ }

	Lexer::Lexer(std::istream& stream)
		: m_Stream(stream)
	{
		CollectTokens();
	}

	std::vector<jsonc::Token> Lexer::GetTokens()
	{
		return m_Tokens;
	}

	void Lexer::CollectTokens()
	{
		Token token(TokenType::NONE);

		do 
		{
			token = NextToken();
			m_Tokens.push_back(token);
		} while (token.Type != TokenType::END_OF_FILE);
	}

	jsonc::Token Lexer::NextToken()
	{
		char c;

		if (!(m_Stream >> c))
			return Token(TokenType::END_OF_FILE);

		switch (c)
		{
		case '{': return Token(TokenType::CURLY_OPEN);
		case '}': return Token(TokenType::CURLY_CLOSE);
		case ':': return Token(TokenType::COLON);
		case '[': return Token(TokenType::ARRAY_OPEN);
		case ']': return Token(TokenType::ARRAY_CLOSE);
		case ',': return Token(TokenType::COMMA);
		}

		if (c == '\"')
		{
			// Collect string

			std::stringstream ss;
			while (m_Stream >> c && c != '\"')
				ss << c;

			return Token(TokenType::STRING, ss.str());
		}
		else if (std::isdigit(c) || c == '-')
		{
			double number;
			m_Stream.putback(c);
			m_Stream >> number;
			return Token(TokenType::NUMBER, number);
		}
		else if (c == 'n')
		{
			// Check for "null"

			m_Stream.putback(c);

			char nullArr[5] = { 0 };

			m_Stream.width(5);
			m_Stream >> nullArr;
			m_Stream.width(1);

			if (std::strcmp(nullArr, "null") == 0)
				return Token(TokenType::NULL_TYPE);
		}
		else if (c == 't' || c == 'f') 
		{
			// true or false

			m_Stream.putback(c);

			bool boolean;
			m_Stream >> std::boolalpha >> boolean >> std::noboolalpha;
			return Token(TokenType::BOOLEAN, boolean);
		}

		return Token(TokenType::NONE);
	}
}