#include <iostream>
#include <functional>
#include <string>

#include "Monk.h"

#include <windows.h>

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

using namespace monk;
using namespace monk::gfx;

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	void OnEvent(Event& e);

private:
	Window* m_Window;
};

Application::Application()
{
	m_Window = new Window(1280, 720, "Monk");
	m_Window->SetEventCallback(BIND_FUNCTION(Application::OnEvent));
	m_Window->SwapInterval(1);

	if (!utils::OpenGLLoader::LoadOpenGL(utils::OpenGLVersion::OPENGL_3_3))
		DIE("Failed to load OpenGL functions");
	else
	{
		LOG_INFO("OpenGL: ");
		LOG_INFO("	vendor: {0}", glGetString(GL_VENDOR));
		LOG_INFO("	renderer: {0}", glGetString(GL_RENDERER));
		LOG_INFO("	version: {0}", glGetString(GL_VERSION));
	}


	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

Application::~Application()
{
	delete m_Window;
}

void Application::Run()
{
	float data[] = {
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	uint32_t vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	VertexBuffer vertexBuffer(data, sizeof(data));
	vertexBuffer.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	const char* vertexSrc = R"(
		#version 330 core
		layout (location = 0) in vec3 a_Position;
		void main()
		{
			gl_Position = vec4(a_Position, 1.0f);
		}
	)";

	const char* fragmentSrc = R"(
		#version 330 core
		out vec4 FragColor;
		void main()
		{
			FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	)";

	Shader shader(vertexSrc, fragmentSrc);
	shader.Bind();

	while (!m_Window->Closed())
	{
		if (Input::IsKeyPressed(Key::Escape))
			m_Window->Close();

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		m_Window->Update();
	}
}

void Application::OnEvent(Event& e)
{

}

int main()
{
	Application app;
	app.Run();

	return 0;
}
