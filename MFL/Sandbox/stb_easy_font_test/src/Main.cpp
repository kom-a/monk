#include "stb_easy_font.h"

#include <stdint.h>
#include <iostream>

#include <MWL/MWL.h> 
#include <MOGL/MOGL.h>

#include "Shader.h"

const char* g_VertexShaderSource = R"(
	#version 430 core

	layout(location = 0) in vec3 a_pos;
	layout(location = 1) in uint a_col;

	out vec4 v_col;

	uniform mat4 matrix;

	void main()
	{
		gl_Position = matrix * vec4(a_pos, 1.0f);
		
		uint r = (a_col >> 24) & 0xFF;
		uint g = (a_col >> 16) & 0xFF;
		uint b = (a_col >> 8) & 0xFF;
		uint a = (a_col >> 0) & 0xFF;
		
		v_col = vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
)";

const char* g_FragmentShaderSource = R"(
	#version 430 core

	out vec4 FragColor;

	in vec4 v_col;

	void main()
	{
		FragColor = v_col;
	}
)";

int main()
{
	mwl::WindowProps windowProps;
	windowProps.Title = L"stb_easy_font";

	mwl::SetOpenGLVersion(mwl::OpenGLVersion::OPENGL_4_6);
	mwl::Window* window = mwl::Create(windowProps);

	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		std::cout << "Failed to load opengl" << std::endl;

	monk::Shader shader(g_VertexShaderSource, g_FragmentShaderSource);
	
	char* text = "monk";

	int width = stb_easy_font_width(text);
	int height = stb_easy_font_height(text);

	size_t size = 10000000;
	uint8_t* buffer = new uint8_t[size];
	unsigned char color[4] = { 255, 255, 255, 255 };

	uint32_t count = stb_easy_font_print(0, 0, text, color , buffer, size);

	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	uint32_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	uint32_t vertex_size = (sizeof(float) * 3 + sizeof(uint32_t));
	uint32_t buffer_size = count * 4 * vertex_size;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_size, 0);
	glVertexAttribPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, vertex_size, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	uint32_t IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	std::vector<uint32_t> indices;
	uint32_t currentIndex = 0;
	for (size_t i = 0; i < count; i++)
	{
		indices.push_back(currentIndex + 0u);
		indices.push_back(currentIndex + 1u);
		indices.push_back(currentIndex + 2u);

		indices.push_back(currentIndex + 0u);
		indices.push_back(currentIndex + 2u);
		indices.push_back(currentIndex + 3u);

		currentIndex += 4;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

	const auto L = 0;
	const auto R = window->GetWidth();
	const auto T = window->GetHeight();
	const auto B = 0;


	float matrix[16] = {
		2.0f / (R - L)		, 0.0f				, 0.0f	, 0.0f,
		0.0f				, 2.0f / (T - B)	, 0.0f	, 0.0f,
		0.0f				, 0.0f				, -1.0f	, 0.0f,
		(R + L) / (L - R)	, (T + B) / (B - T)	, 0.0f	, 1.0f
	};

	while (!window->Closed())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, window->GetWidth(), window->GetHeight());

		shader.Bind();
		
		auto location = glGetUniformLocation(shader.GetID(), "matrix");
		glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

		window->Update();
		window->SwapBuffers();
	}

	return 0;
}