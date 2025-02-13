#include "Shader.h"

#include <MOGL/MOGL.h>

namespace mui
{
	Shader::Shader(std::string_view vertexSource, std::string_view fragmentSource)
	{
		m_ShaderID = glCreateProgram();
		uint32_t vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
		uint32_t fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
		glAttachShader(m_ShaderID, vertexShader);
		glAttachShader(m_ShaderID, fragmentShader);
		glLinkProgram(m_ShaderID);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ShaderID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(std::string_view name, int value)
	{
		glUseProgram(m_ShaderID);
		int location = glGetUniformLocation(m_ShaderID, name.data());
		if (location != -1)
			glUniform1i(location, value);
	}

	void Shader::SetMatrix4(std::string_view name, const std::array<float, 16>& matrix)
	{
		glUseProgram(m_ShaderID);
		int location = glGetUniformLocation(m_ShaderID, name.data());
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data());
	}

	uint32_t Shader::CompileShader(std::string_view shaderSource, uint32_t type)
	{
		uint32_t shader = glCreateShader(type);

		const char* src = shaderSource.data();
		glShaderSource(shader, 1, &src, 0);
		glCompileShader(shader);

		return shader;
	}
}