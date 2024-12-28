#include "graphics/Shader.h"

#include "core/Log.h"
#include <MOGL/MOGL.h>

#define INVALID ((uint32_t) -1)

namespace monk
{
	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
	{
		m_ShaderID = glCreateProgram();
		uint32_t vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
		uint32_t fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
		glAttachShader(m_ShaderID, vertexShader);
		glAttachShader(m_ShaderID, fragmentShader);
		glLinkProgram(m_ShaderID);
		CheckError(m_ShaderID, GL_LINK_STATUS);
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

	void Shader::SetInt(const std::string& name, int value)
	{
		glUseProgram(m_ShaderID);
		int location = glGetUniformLocation(m_ShaderID, name.c_str());
		if (location != -1)
			glUniform1i(location, value);
		else
			LOG_WARNING("Failed to set uniform: {0}", name);
	}
	
	void Shader::SetMatrix4(const std::string& name, const mml::mat4& matrix)
	{
		glUseProgram(m_ShaderID);
		int location = glGetUniformLocation(m_ShaderID, name.c_str());
		if (location != -1)
			glUniformMatrix4fv(location, 1, GL_TRUE, &matrix[0][0]);
		else
			LOG_WARNING("Failed to set uniform: {0}", name);
	}

	Ref<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		return CreateRef<Shader>(vertexSource, fragmentSource);
	}

	uint32_t Shader::CompileShader(const std::string& shaderSource, uint32_t type)
	{
		uint32_t shader = glCreateShader(type);

		const char* src = shaderSource.c_str();
		glShaderSource(shader, 1, &src, 0);
		glCompileShader(shader);
		CheckError(shader, GL_COMPILE_STATUS);
		
		return shader;
	}

	void Shader::CheckError(uint32_t object, uint32_t status)
	{
		switch (status)
		{
			case GL_COMPILE_STATUS:
			{
				int success;
				glGetShaderiv(object, status, &success);
				if (!success)
				{
					int length;
					glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
					char* infoLog = (char*)alloca(length);
					glGetShaderInfoLog(object, length, &length, infoLog);
					LOG_ERROR(infoLog);
				}
			} break;
			case GL_LINK_STATUS:
			{
				int success;
				glGetProgramiv(object, status, &success);
				if (!success)
				{
					int length;
					glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
					char* infoLog = (char*)alloca(length);
					glGetProgramInfoLog(object, length, &length, infoLog);
					LOG_ERROR(infoLog);
				}
			} break;
		}
	}
}