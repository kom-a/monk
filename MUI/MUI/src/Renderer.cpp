#include "Renderer.h"

#include <string>
#include <MOGL/MOGL.h>

#include <iostream>

namespace mui
{
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
		const std::string vertexSrc = R"(#version 330 core
			layout (location = 0) in vec2 a_Position;
			layout (location = 1) in vec4 a_Color;

			out vec4 v_Color;

			void main()
			{
				gl_Position = vec4(a_Position, 0.0f, 1.0f);
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

		const char* vertexCSrc= vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &vertexCSrc, 0);
		glCompileShader(vertexShader);
		CheckError(vertexShader, GL_COMPILE_STATUS);

		const char* fragment = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &fragment, 0);
		glCompileShader(fragmentShader);
		CheckError(vertexShader, GL_COMPILE_STATUS);

		m_Shader = glCreateProgram();
		glAttachShader(m_Shader, vertexShader);
		glAttachShader(m_Shader, fragmentShader);
		glLinkProgram(m_Shader);
		CheckError(m_Shader, GL_LINK_STATUS);
	}

	void Renderer::DrawRect(Vec2i position, Vec2i size, Vec4i color)
	{
		unsigned int m_VAO;
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		float pos_x = position.X / 1600.0f;
		float pos_y = position.Y / 900.0f;

		float size_x = size.X / 1600.0f;
		float size_y = size.Y / 900.0f;

		float vertices[] = {
		   pos_x, pos_y,						color.X, color.Y, color.Z, color.W,
		   pos_x + size_x, pos_y,				color.X, color.Y, color.Z, color.W,
		   pos_x, pos_y + size_y,				color.X, color.Y, color.Z, color.W,
		
		   pos_x + size_x, pos_y,				color.X, color.Y, color.Z, color.W,
		   pos_x, pos_y + size_y,				color.X, color.Y, color.Z, color.W,
		   pos_x + size_x, pos_y + size_y,	color.X, color.Y, color.Z, color.W,
		};

		GLuint points_vbo = 0;
		glGenBuffers(1, &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		while (glGetError() != 0)
		{
			int a = 0;
		}
	}
}