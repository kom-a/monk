#pragma once

#include <string_view>
#include <array>

namespace mui
{
	class Shader
	{
	public:
		Shader(std::string_view vertexSource, std::string_view fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetInt(std::string_view name, int value);
		void SetMatrix4(std::string_view, const std::array<float, 16>& matrix);

	private:
		uint32_t CompileShader(std::string_view shaderSource, uint32_t type);

	private:
		uint32_t m_ShaderID;
	};
}