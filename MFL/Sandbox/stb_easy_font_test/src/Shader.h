#pragma once

#include <inttypes.h>
#include <string>

namespace monk
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value);

		uint32_t GetID() { return m_ShaderID; }

	private:
		uint32_t CompileShader(const std::string& shaderSource, uint32_t type);
		void CheckError(uint32_t object, uint32_t status);

	private:
		uint32_t m_ShaderID;
	};
}