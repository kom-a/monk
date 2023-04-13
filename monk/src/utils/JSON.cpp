#include "JSON.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "core/Log.h"

#define STRINGIFY(str) #str

namespace monk
{
	std::shared_ptr<JSONObject> CollectObject();

	static char C;
	static std::ifstream Stream;

	enum class TokenType
	{
		CURLY_OPEN,
		CURLY_CLOSE,
		COLON,
		STRING,
		NUMBER,
		ARRAY_OPEN,
		ARRAY_CLOSE,
		COMMA,
		BOOLEAN,
		NULL_TYPE
	};

	struct Token
	{
		TokenType Type;
		std::string Value;

		std::string ToString()
		{
			switch (Type)
			{
			case TokenType::CURLY_OPEN: return STRINGIFY(TokenType::CURLY_OPEN);
			case TokenType::CURLY_CLOSE: return STRINGIFY(TokenType::CURLY_CLOSE);
			case TokenType::COLON: return STRINGIFY(TokenType::COLON);
			case TokenType::STRING: return STRINGIFY(TokenType::STRING) "\nValue: " + Value;
			case TokenType::NUMBER: return STRINGIFY(TokenType::NUMBER) "\nValue: " + Value;
			case TokenType::ARRAY_OPEN: return STRINGIFY(TokenType::ARRAY_OPEN);
			case TokenType::ARRAY_CLOSE: return STRINGIFY(TokenType::ARRAY_CLOSE);
			case TokenType::COMMA: return STRINGIFY(TokenType::COMMA);
			case TokenType::BOOLEAN: return STRINGIFY(TokenType::BOOLEAN) "\nValue: " + Value;
			case TokenType::NULL_TYPE: return STRINGIFY(TokenType::NULL_TYPE);
			default: throw std::logic_error("Unknown TokenType");
			}

			return "Unknown";
		}
	};

	bool Open(const std::string& filename)
	{
		Stream.open(filename, std::ios_base::in);
		if (!Stream.is_open())
			return false;

		return true;
	}

	bool Get()
	{
		return (bool)Stream.get(C);
	}

	void EatWhitespaces()
	{
		while(std::isspace(C))
			if (!Get())
				throw std::logic_error("Unexpectable end of file");
	}

	std::string CollectAlpha()
	{
		std::stringstream ss;

		while (std::isalpha(C))
		{
			ss << C;
			Get();
		}

		return ss.str();
	}

	std::string CollectString()
	{
		if (C != '"')
			throw std::logic_error("Not a string");

		std::stringstream ss;

		Get();
		while (C != '"')
		{
			if (C == '\\')
			{
				ss << C;
				Get();
			}

			ss << C;
			Get();
		}

		Get();
		return ss.str();
	}

	std::string CollectNumber()
	{
		if (!(std::isdigit(C) || C == '-'))
			throw std::logic_error("Not a number");

		std::stringstream ss;
		ss << C;

		Get();
		while (std::isdigit(C))
		{
			ss << C;
			Get();
		}

		if (C == '.')
		{
			ss << C;
			Get();
			if (std::isdigit(C))
			{
				ss << C;
				Get();
				while (std::isdigit(C))
				{
					ss << C;
					Get();
				}
			}
			else
			{
				throw std::logic_error("Incorrect number");
			}
		}

		if (C == 'E' || C == 'e')
		{
			ss << C;
			Get();
			if (C == '+' || C == '-')
			{
				ss << C;
				Get();
			}
			
			while (std::isdigit(C))
			{
				ss << C;
				Get();
			}
		}

		return ss.str();
	}

	Token Next()
	{
		Token token;
		EatWhitespaces();

		if (C == '{')
		{
			token.Type = TokenType::CURLY_OPEN;
			Get();
		}
		else if (C == '}')
		{
			token.Type = TokenType::CURLY_CLOSE;
			Get();
		}
		else if (C == ':')
		{
			token.Type = TokenType::COLON;
			Get();
		}
		else if (C == '"')
		{
			token.Type = TokenType::STRING;
			token.Value = CollectString();
		}
		else if (std::isdigit(C) || C == '-')
		{
			token.Type = TokenType::NUMBER;
			token.Value = CollectNumber();
		}
		else if (C == '[')
		{
			token.Type = TokenType::ARRAY_OPEN;
			Get();
		}
		else if (C == ']')
		{
			token.Type = TokenType::ARRAY_CLOSE;
			Get();
		}
		else if (C == ',')
		{
			token.Type = TokenType::COMMA;
			Get();
		}
		else if (std::isalpha(C))
		{
			std::string alpha = CollectAlpha();
			if (alpha == "true" || alpha == "false")
			{
				token.Type = TokenType::BOOLEAN;
				token.Value = alpha;
			}
			else if (alpha == "null")
			{
				token.Type = TokenType::NULL_TYPE;
			}
		}
		else
		{
			throw std::logic_error("Unknown token");
		}

		return token;
	}

	void EatToken(TokenType type)
	{
		Token token = Next();
		if (token.Type != type)
			throw std::logic_error("Unexpected token");
	}

	std::shared_ptr<JSONList> CollectArray()
	{
		std::shared_ptr<JSONList> list = std::make_shared<JSONList>();

		Token token = Next();

		while (token.Type != TokenType::ARRAY_CLOSE)
		{
			std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();
			switch (token.Type)
			{
				case TokenType::CURLY_OPEN:
				{
					node->Value = CollectObject();
				} break;
				case TokenType::STRING:
				{
					node->Value = std::make_shared<std::string>(token.Value);
				} break;
				case TokenType::NUMBER:
				{
					node->Value = std::stof(token.Value);
				} break;
				case TokenType::ARRAY_OPEN:
				{
					node->Value = CollectArray();
				} break;
				case TokenType::BOOLEAN:
				{
					bool b;
					std::istringstream(token.Value) >> std::boolalpha >> b;
					node->Value = b;
				} break;
				case TokenType::NULL_TYPE:
				{
					node->Value = nullptr;
				} break;
				default:
					throw std::logic_error("Unexpected token");
			}

			list->push_back(node);

			token = Next();
			if (token.Type == TokenType::ARRAY_CLOSE)
				continue;
			if (token.Type != TokenType::COMMA)
				throw std::logic_error("Unexpected token");

			token = Next();
		}
		
		return list;
	}

	std::shared_ptr<JSONObject> CollectObject()
	{
		std::shared_ptr<JSONObject> object = std::make_shared<JSONObject>();
		
		Token token = Next();

		while (token.Type != TokenType::CURLY_CLOSE)
		{
			if (token.Type == TokenType::STRING)
			{
				std::string key = token.Value;
				std::shared_ptr<JSONNode> node = std::make_shared<JSONNode>();

				EatToken(TokenType::COLON);
				token = Next();

				switch (token.Type)
				{
				case TokenType::CURLY_OPEN:
				{
					node->Value = CollectObject();
				} break;
				case TokenType::STRING: 
				{
					node->Value = std::make_shared<std::string>(token.Value);
				} break;
				case TokenType::NUMBER:
				{
					node->Value = std::stof(token.Value);
				} break;
				case TokenType::ARRAY_OPEN:
				{
					node->Value = CollectArray();
				} break;
				case TokenType::BOOLEAN:
				{
					bool b;
					std::istringstream(token.Value) >> std::boolalpha >> b;
					node->Value = b;
				} break;
				case TokenType::NULL_TYPE:
				{
					node->Value = nullptr;
				} break;
				default:
					throw std::logic_error("Unexpected token");
				}

				object->insert(std::make_pair(key, node));
			}
			else
				throw std::logic_error("Unexpected token");

			token = Next();
			if (token.Type == TokenType::CURLY_CLOSE)
				continue;
			if (token.Type != TokenType::COMMA)
				throw std::logic_error("Unexpected token");

			token = Next();
		}

		return object;
	}

	std::shared_ptr<const JSONNode> JSON::ParseFile(const std::string& filename)
	{
		if(Stream.is_open())
			Stream.close();
		
		if (!Open(filename))
			throw std::logic_error("Failed to open file");

		std::shared_ptr<JSONNode> json = std::make_shared<JSONNode>();

		Get();
		if (Next().Type == TokenType::CURLY_OPEN)
			json->Value = CollectObject();
		else
		{
			Stream.close();
			throw std::logic_error("Not a JSON object");
		}
		
		Stream.close();
		return json;
	}

	const JSONObject& JSONNode::GetJSONObject() const
	{
		if (auto value = std::get_if<std::shared_ptr<const JSONObject>>(&Value))
		{
			return **value;
		}
		else
			throw std::logic_error("Not a JSONObject");
	}

	const JSONList& JSONNode::GetList() const
	{
		if (auto value = std::get_if<std::shared_ptr<const JSONList>>(&Value))
		{
			return **value;
		}
		else
			throw std::logic_error("Not a JSONList");
	}

	const std::string& JSONNode::GetString() const
	{
		if (auto value = std::get_if<std::shared_ptr<std::string>>(&Value))
		{
			return **value;
		}
		else
			throw std::logic_error("Not a string");
	}

	float JSONNode::GetNumber() const
	{
		if (auto value = std::get_if<float>(&Value))
		{
			return *value;
		}
		else
			throw std::logic_error("Not a number");
	}

	bool JSONNode::GetBoolean() const
	{
		if (auto value = std::get_if<bool>(&Value))
		{
			return *value;
		}
		else
			throw std::logic_error("Not a boolean");
	}

	std::string JSONNode::TryGetString(const std::string& key, const std::string& default) const
	{
		if (auto value = std::get_if<std::shared_ptr<const JSONObject>>(&Value))
		{
			auto object = **value;
			if (object.find(key) != object.end())
				return (*object.at(key)).GetString();
			else
				return default;
		}
		else
			throw std::logic_error("Not a JSONObject");
	}

	float JSONNode::TryGetNumber(const std::string& key, float default) const
	{
		if (auto value = std::get_if<std::shared_ptr<const JSONObject>>(&Value))
		{
			auto object = **value;
			if (object.find(key) != object.end())
				return (*object.at(key)).GetNumber();
			else
				return default;
		}
		else
			throw std::logic_error("Not a JSONObject");
	}

	bool JSONNode::TryGetBoolean(const std::string& key, bool default) const
	{
		if (auto value = std::get_if<std::shared_ptr<const JSONObject>>(&Value))
		{
			auto object = **value;
			if (object.find(key) != object.end())
				return (*object.at(key)).GetBoolean();
			else
				return default;
		}
		else
			throw std::logic_error("Not a JSONObject");
	}

	bool JSONNode::Has(const std::string& key) const
	{
		if (auto value = std::get_if<std::shared_ptr<const JSONObject>>(&Value))
		{
			return (**value).find(key) != (**value).end();
		}
		else
			throw std::logic_error("Not a JSONObject");
	}

	const monk::JSONNode& JSONNode::operator[](const std::string& key) const
	{
		if (auto ptr = std::get_if<std::shared_ptr<const JSONObject>>(&Value))
		{
			auto& object = **ptr;
			auto value = object.at(key);
			return *value;
		}
		else
			throw std::logic_error("Not a JSONObject");
	}

	const monk::JSONNode& JSONNode::operator[](size_t index) const
	{
		if (auto ptr = std::get_if<std::shared_ptr<const JSONList>>(&Value))
		{
			auto& object = **ptr;
			auto value = object.at(index);
			return *value;
		}
		else
			throw std::logic_error("Not a JSONList");
	}

}
