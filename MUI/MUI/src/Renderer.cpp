#include "Renderer.h"

#include <string>
#include <MOGL/MOGL.h>

#include <iostream>

namespace mui
{
	uint32_t Renderer::RenderSettings::MaxVertices = 4096;

	static void CheckError(uint32_t object, uint32_t status)
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
				std::cout << infoLog << std::endl;
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
				std::cout << infoLog << std::endl;
			}
		} break;
		}
	}

	Renderer::Renderer()
	{
		m_Shader	= CreateShader();
		m_VAO		= CreateVertexArrayObject();
		m_VBO		= CreateVertexBufferObject();
	}

	void Renderer::BeginDraw()
	{
		m_Buffer.clear();
	}

	void Renderer::DrawRect(Vec2f position, Vec2f size, Vec4f color)
	{
		const uint32_t rectVerticesCount = 6;
		if (m_Buffer.size() + rectVerticesCount >= RenderSettings::MaxVertices)
			Flush();

		float z = 0;

		Vertex v1 = Vertex(Vec3f(position.X			, position.Y			, z), color);
		Vertex v2 = Vertex(Vec3f(position.X + size.X, position.Y			, z), color);
		Vertex v3 = Vertex(Vec3f(position.X + size.X, position.Y + size.Y	, z), color);
		Vertex v4 = Vertex(Vec3f(position.X			, position.Y + size.X	, z), color);

		m_Buffer.push_back(v1);
		m_Buffer.push_back(v2);
		m_Buffer.push_back(v3);

		m_Buffer.push_back(v1);
		m_Buffer.push_back(v3);
		m_Buffer.push_back(v4);
	}

	void Renderer::EndDraw()
	{
		Flush();
	}

	void Renderer::Flush()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Buffer.size() * sizeof(Vertex), m_Buffer.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glUseProgram(m_Shader);

		glDrawArrays(GL_TRIANGLES, 0, (int)m_Buffer.size());

		m_Buffer.clear();
	}

	unsigned int Renderer::CreateShader() const
	{
		const std::string vertexSrc = R"(#version 330 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec4 a_Color;

			out vec4 v_Color;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0f);
				v_Color = a_Color;
			}
		)";

		const std::string fragmentSrc = R"(#version 330 core
			out vec4 FragColor;

			in vec4 v_Color;

			void main()
			{
				FragColor = v_Color;
			}
		)";

		uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
		uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vertexCSrc = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &vertexCSrc, nullptr);
		glCompileShader(vertexShader);
		CheckError(vertexShader, GL_COMPILE_STATUS);

		const char* fragment = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &fragment, nullptr);
		glCompileShader(fragmentShader);
		CheckError(fragmentShader, GL_COMPILE_STATUS);

		unsigned int shader = glCreateProgram();
		glAttachShader(shader, vertexShader);
		glAttachShader(shader, fragmentShader);
		glLinkProgram(shader);
		CheckError(shader, GL_LINK_STATUS);

		return shader;
	}

	uint32_t Renderer::CreateVertexArrayObject() const
	{
		uint32_t vertexArrayObject = 0;
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		return vertexArrayObject;
	}

	uint32_t Renderer::CreateVertexBufferObject() const
	{
		uint32_t vertexBufferObject = 0;
		glGenBuffers(1, &vertexBufferObject);

		return vertexBufferObject;
	}

}