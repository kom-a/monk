#include "../include/Jsonetic/Jsonetic.h"

#include <stdexcept>
#include <sstream>
#include <fstream>

#include "Lexer.h"

// TODO: DELETE THIS include
#include <iostream>
#define STRINGIFY(str) #str

namespace jsonc
{
	Object::Object(std::initializer_list<std::pair<const std::string, Value>> init) 
		: m_Map(init)
	{ }

	jsonc::Value& Object::operator[](const std::string& key)
	{
		return m_Map[key];
	}

	const jsonc::Value& Object::operator[](const std::string& key) const
	{
		auto it = m_Map.find(key);
		if (it != m_Map.end())
			return it->second;
		throw std::out_of_range("Key not found in JSON object.");
	}

	std::string Object::Dump() const
	{
		std::stringstream ss;
		DumpImpl(ss, 0);

		return ss.str();
	}

	bool Object::Has(const std::string& key) const
	{
		return m_Map.find(key) != m_Map.end();
	}

	static void Indent(std::stringstream& ss, size_t indent)
	{
		for (size_t i = 0; i < indent * 4; i++)
		{
			ss << " ";
		}
	}

	void Object::DumpImpl(std::stringstream& ss, size_t indent) const
	{
		size_t index = 0;

		ss << "{\n";

		for (const auto& [key, value] : m_Map)
		{
			Indent(ss, indent + 1);
			ss << "\"" << key << "\": ";
			value.DumpImpl(ss, indent);

			if (index < m_Map.size() - 1)
				ss << ",";

			ss << std::endl;

			index++;
		}

		Indent(ss, indent);
		ss << "}";
	}

	std::ostream& operator << (std::ostream& stream, const Object& obj)
	{
		return stream << obj.Dump();
	}

	Value::Value()
		: m_Data(nullptr)
	{ }

	Value::Value(bool b)
		: m_Data(b)
	{ }


	Value::Value(int i)
		: m_Data((double)i)
	{ }

	Value::Value(double d)
		: m_Data(d)
	{ }

	Value::Value(const char* s) 
		: m_Data(std::string(s))
	{ }

	Value::Value(const std::string& s) 
		: m_Data(s)
	{ }

	Value::Value(const Object& obj) 
		: m_Data(obj)
	{ }

	Value::Value(const std::vector<Value>& arr)
		: m_Data(arr)
	{ }

	Value::Value(std::initializer_list<std::pair<const std::string, Value>> init)
		: m_Data(Object(init))
	{ }

	jsonc::Value& Value::operator[](const std::string& key)
	{
		if (auto* obj = std::get_if<Object>(&m_Data))
			return (*obj)[key];
		throw std::runtime_error("JSON value is not an object.");
	}

	const jsonc::Value& Value::operator[](const std::string& key) const
	{
		if (auto* obj = std::get_if<Object>(&m_Data))
			return (*obj)[key];
		throw std::runtime_error("JSON value is not an object.");
	}

	void Value::DumpImpl(std::stringstream& ss, size_t indent) const
	{
		if (std::holds_alternative<std::nullptr_t>(m_Data))
			ss << "null";
		else if (std::holds_alternative<bool>(m_Data))
			ss << std::boolalpha << std::get<bool>(m_Data) << std::noboolalpha;
		else if (std::holds_alternative<Number>(m_Data))
			ss << std::fixed << std::get<Number>(m_Data) << std::scientific;
		else if (std::holds_alternative<std::string>(m_Data))
			ss << "\"" << std::get<std::string>(m_Data) << "\"";
		else if (std::holds_alternative<Object>(m_Data))
			std::get<Object>(m_Data).DumpImpl(ss, indent + 1);
		else if (std::holds_alternative<std::vector<Value>>(m_Data))
		{
			const std::vector<Value>& values = std::get<std::vector<Value>>(m_Data);

			ss << "[\n";
			for (size_t i = 0; i < values.size(); i++)
			{
				const Value& v = values[i];
				Indent(ss, indent + 2);
				v.DumpImpl(ss, indent + 1);

				if (i != values.size() - 1)
					ss << "," << std::endl;
			}
			ss << "\n";
			Indent(ss, indent + 1);
			ss << "]";
		}
	}

	static std::ostream& operator<< (std::ostream& stream, const TokenType& tokenType)
	{
		switch (tokenType)
		{
		case TokenType::NONE			: stream << STRINGIFY(TokenType::NONE);			break;
		case TokenType::CURLY_OPEN		: stream << STRINGIFY(TokenType::CURLY_OPEN);	break;
		case TokenType::CURLY_CLOSE		: stream << STRINGIFY(TokenType::CURLY_CLOSE);	break;
		case TokenType::COLON			: stream << STRINGIFY(TokenType::COLON);		break;
		case TokenType::STRING			: stream << STRINGIFY(TokenType::STRING);		break;
		case TokenType::NUMBER			: stream << STRINGIFY(TokenType::NUMBER);		break;
		case TokenType::ARRAY_OPEN		: stream << STRINGIFY(TokenType::ARRAY_OPEN);	break;
		case TokenType::ARRAY_CLOSE		: stream << STRINGIFY(TokenType::ARRAY_CLOSE);	break;
		case TokenType::COMMA			: stream << STRINGIFY(TokenType::COMMA);		break;
		case TokenType::BOOLEAN			: stream << STRINGIFY(TokenType::BOOLEAN);		break;
		case TokenType::NULL_TYPE		: stream << STRINGIFY(TokenType::NULL_TYPE);	break;
		}

		return stream;
	}

	struct TokenBuffer
	{
		const std::vector<Token>& Tokens;
		size_t Index;

		TokenBuffer(const std::vector<Token>& tokens)
			: Tokens(tokens), Index(0) {}

		const Token& Next()
		{
			// Return last token on overflow
			if (Tokens.size() == Index + 1)
				return Tokens[Index];

			return Tokens[Index++];
		}

		const Token& Next(TokenType type)
		{
			const Token& token = Next();

			if (token.Type != type)
				throw std::logic_error("Unexpected token");

			return token;
		}

		const Token& Peek()
		{
			return Tokens[Index];
		}

		void Eat(TokenType type)
		{
			Next(type);
		}
	};

	static Object CollectObject(TokenBuffer& buffer);
	static std::vector<Value> CollectArray(TokenBuffer& buffer);

	static std::vector<Value> CollectArray(TokenBuffer& buffer)
	{
		std::vector<Value> result;

		buffer.Eat(TokenType::ARRAY_OPEN);

		while (buffer.Peek().Type != TokenType::ARRAY_CLOSE)
		{
			const Token& value = buffer.Peek();

			switch (value.Type)
			{
				case TokenType::STRING:
				{
					result.push_back(Value(value.StringData));
					buffer.Eat(TokenType::STRING);
				} break;
				case TokenType::NUMBER:
				{
					result.push_back(Value(value.NumberData));
					buffer.Eat(TokenType::NUMBER);
				} break;
				case TokenType::BOOLEAN:
				{
					result.push_back(Value(value.BooleanData));
					buffer.Eat(TokenType::BOOLEAN);
				} break;
				case TokenType::NULL_TYPE:
				{
					result.push_back(Value());
					buffer.Eat(TokenType::NULL_TYPE);
				} break;
				case TokenType::CURLY_OPEN:
				{
					result.push_back(CollectObject(buffer));
				} break;
				case TokenType::ARRAY_OPEN:
				{
					result.push_back(CollectArray(buffer));
				} break;
			}

			if (buffer.Peek().Type != TokenType::ARRAY_CLOSE)
				buffer.Eat(TokenType::COMMA);
		}

		buffer.Eat(TokenType::ARRAY_CLOSE);

		return result;
	}
	
	static Object CollectObject(TokenBuffer& buffer)
	{
		Object obj;

		buffer.Eat(TokenType::CURLY_OPEN);

		while (buffer.Peek().Type != TokenType::CURLY_CLOSE)
		{
			const Token& name = buffer.Next(TokenType::STRING);
			buffer.Eat(TokenType::COLON);
			const Token& value = buffer.Peek();

			switch (value.Type)
			{
				case TokenType::STRING:
				{
					obj[name.StringData] = Value(value.StringData);
					buffer.Eat(TokenType::STRING);
				} break;
				case TokenType::NUMBER:
				{
					obj[name.StringData] = Value(value.NumberData);
					buffer.Eat(TokenType::NUMBER);
				} break;
				case TokenType::BOOLEAN:
				{
					obj[name.StringData] = Value(value.BooleanData);
					buffer.Eat(TokenType::BOOLEAN);
				} break;
				case TokenType::NULL_TYPE:
				{
					obj[name.StringData] = Value();
					buffer.Eat(TokenType::NULL_TYPE);
				} break;
				case TokenType::CURLY_OPEN:
				{
					obj[name.StringData] = CollectObject(buffer);
				} break;
				case TokenType::ARRAY_OPEN:
				{
					obj[name.StringData] = CollectArray(buffer);
				} break;
			}

			if(buffer.Peek().Type != TokenType::CURLY_CLOSE)
				buffer.Eat(TokenType::COMMA);
		}

		buffer.Eat(TokenType::CURLY_CLOSE);

		return obj;
	}

	Object Parse(std::istream& stream)
	{
		std::vector<Token> tokens;

		try
		{
			Lexer lexer(stream);
			tokens = lexer.GetTokens();
		}
		catch (const std::exception&)
		{
			// TODO: Handle errors
			return {};
		}

		try
		{
			TokenBuffer buffer(tokens);
			Object result = CollectObject(buffer);

			return result;
		}
		catch (const std::exception& e)
		{
			// TODO: Handle errors

			std::cerr << e.what() << std::endl;

			return {};
		}
	}

	Object Parse(const std::string& source)
	{
		std::istringstream stream(source);

		return Parse(stream);
	}

	Object ParseFile(const std::string& filename)
	{
		std::ifstream ifs(filename);
		if (!ifs.is_open())
			return {};

		return Parse(ifs);
	}

}
