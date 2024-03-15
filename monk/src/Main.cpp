#include <iostream>
#include <functional>
#include <string>
#include <chrono>

#include <MWL/MWL.h>
#include <MOGL/MOGL.h>

#include "core/Memory.h"
#include "core/Log.h"

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

using namespace monk;

class Application
{
public:
	Application();
	~Application() = default;

	void Run();

private:
	void Update(float deltaTime);

private:
	mwl::Window* m_Window;
};

Application::Application()
{
	mwl::SetOpenGLVersion(mwl::OpenGLVersion::OPENGL_4_6);
	m_Window = mwl::Create();

	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		LOG_ERROR("Failed to load opengl");
	else
		LOG_INFO("\nOpenGL: \n\tvendor: {0}\n\trenderer: {1}\n\tversion: {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
}

void Application::Run()
{
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

	while (!m_Window->Closed())
	{
		glClear(GL_COLOR_BUFFER_BIT);

		m_Window->Update();
	}
}

int main()
{
	Application app;
	app.Run();

	return 0;
}
