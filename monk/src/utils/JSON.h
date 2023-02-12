#pragma once

#include <variant>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

namespace monk
{
	class JSONNode;
	using JSONObject = std::unordered_map<std::string, std::shared_ptr<JSONNode>>;
	using JSONList = std::vector<std::shared_ptr<JSONNode>>;

	class JSONNode
	{
	public:
		JSONNode()
		{
			Value = false;
		}

		const JSONObject& GetJSONObject() const;
		const JSONList& GetList() const;
		const std::string& GetString() const;
		float GetNumber() const;
		bool GetBoolean() const;

		const JSONNode& operator[](const std::string& key) const;
		const JSONNode& operator[](size_t index) const;

	public:
		std::variant<std::shared_ptr<const JSONObject>, std::shared_ptr<const JSONList>, std::shared_ptr<std::string>, float, bool, std::nullptr_t> Value;
	};

	class JSON
	{
	public:
		static std::shared_ptr<const JSONNode> ParseFile(const std::string & filename);
	};
}
