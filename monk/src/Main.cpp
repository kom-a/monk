#include <iostream>
#include <chrono>

#include "core/Window.h"
#include "core/Log.h"

#include "utils/OpenGL.h"

#include "math/math.h"

int main()
{
	using namespace monk;
	using namespace monk::utils;

	Window window(1280, 720, "Monk");

	if (!OpenGLLoader::LoadOpenGL(OpenGLVersion::OPENGL_3_3))
		DIE("Fauiled to load OpenGL functions");
	else
		LOG_INFO("OpenGL version: {0}", glGetString(GL_VERSION));

	// NOTE: 
	// Swap interval is not set to window instance. Maybe change it to Window::SwapInterval? 
	window.SwapInterval(1);

	unsigned vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	unsigned vertex_object;
	glGenBuffers(1, &vertex_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_object);

	float buffer[] = {
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	float bufferOrtho[] = {
		50.0f, 25.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		75.0f, 75.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		25.0f, 75.0f, 0.0f, 	0.0f, 0.0f, 1.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glClearColor(0.3f, 0.4f, 0.5f, 1.0f);

	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout(location=0) in vec3 a_Position;\n"
		"layout(location=1) in vec3 a_Color;\n"
		"out vec3 v_Color;"
		"uniform mat4 u_Model;"
		"uniform mat4 u_Projection = mat4(1.0f);"
		"void main()\n"
		"{\n"
		"	gl_Position = u_Projection * u_Model * vec4(a_Position, 1.0f);\n"
		"	v_Color = a_Color;"
		"}\n";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 v_Color;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(v_Color * 2, 1.0f);\n"
		"}\n";

	int success;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
		LOG_ERROR("Failed to compile vertex shader");

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
		LOG_ERROR("Failed to compile fragment shader");

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
		LOG_ERROR("Failed to link program");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(program);

	int modelLocation = glGetUniformLocation(program, "u_Model");
	if (modelLocation == -1)
		LOG_ERROR("Failed to find unifom location");
	int projectionLocation = glGetUniformLocation(program, "u_Projection");
	if(projectionLocation == -1)
		LOG_ERROR("Failed to find unifom location");

	math::mat4 m1(1.0f, 2.0f, 3.0f, 4.0f,
		1.0f, 2.0f, 3.0f, 4.0f,
		1.0f, 2.0f, 3.0f, 4.0f,
		1.0f, 2.0f, 3.0f, 4.0f);

	m1 = math::mat4(1.0f);
	m1.Translate(math::vec3(0, 0, 2));
	
	auto lastTime = std::chrono::system_clock::now();

	while (!window.Closed())
	{
		auto currentTime = std::chrono::system_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
		lastTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT);

		math::mat4 projection = math::Perspective(90.0f, window.GetWidth() / (float)window.GetHeight(), 0.01f, 10.0f);

		glUniformMatrix4fv(modelLocation, 1, GL_TRUE, &m1[0][0]);
		glUniformMatrix4fv(projectionLocation, 1, GL_TRUE, &projection[0][0]);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		window.Update();
	}

	/*glDeleteBuffers(1, &vertex_object);
	glDeleteVertexArrays(1, &vertex_array);*/

	return 0;
}
