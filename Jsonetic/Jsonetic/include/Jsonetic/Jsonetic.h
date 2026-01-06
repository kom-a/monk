#pragma once

#include <string_view>
#include <unordered_map>
#include <variant>
#include <string>
#include <istream>

namespace jsonc {

	class Value;

	using Number = double;
	using Array = std::vector<Value>;

	class Object {
	public:
		Object() = default;
		Object(std::initializer_list<std::pair<const std::string, Value>> init);

		Value& operator[](const std::string& key);
		const Value& operator[](const std::string& key) const;

		std::string Dump() const;
		friend std::ostream& operator<< (std::ostream& stream, const Object& obj);

		bool Has(const std::string& key) const;

	private:
		void DumpImpl(std::stringstream& ss, size_t indent) const;

	private:
		using Map = std::unordered_map<std::string, Value>;
		Map m_Map;

		friend class Value;
	};

	class Value {
	public:
		Value();

		// Constructors for various types:
		Value(const char* s);
		Value(const std::string& s);
		Value(bool b);
		Value(int i);
		Value(Number d);
		Value(const Object& obj);
		Value(const std::vector<Value>& arr);
			
		// Constructor to support nested object initialization directly
		Value(std::initializer_list<std::pair<const std::string, Value>> init);
		
		template<typename T>
		const T& Get() const
		{
			if (const auto* obj = std::get_if<T>(&m_Data))
				return *obj;
			throw std::runtime_error(std::string("JSON value is not an ") + typeid(T).name());
		}

		template<typename T>
		T& Get()
		{
			const T& ret = static_cast<const Value*>(this)->Get<T>();
			return const_cast<T&>(ret);
		}

		// Overload operator[] for non-const access.
		Value& operator[](const std::string& key);

		// Overload operator[] for const access.
		const Value& operator[](const std::string& key) const;

	private:
		friend class Object;
		void DumpImpl(std::stringstream& ss, size_t indent) const;

	private:
		std::variant<std::nullptr_t, bool, Number, std::string, Object, std::vector<Value>> m_Data;
	};

	Object Parse(std::istream& stream);
	Object Parse(const std::string& source);
	Object ParseFile(const std::string& filename);

}