#pragma once

#include <inttypes.h>
#include <string>

namespace monk::gfx
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t CompileShader(const std::string& shaderSource, uint32_t type);
		void CheckError(uint32_t object, uint32_t status);

	private:
		uint32_t m_ShaderID;
	};
}