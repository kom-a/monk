#include <iostream>
#include <functional>

#include "Monk.h"

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

using namespace monk;

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	void OnEvent(Event& e);

	bool OnWindowClose(WindowCloseEvent& e);
	bool OnWindowResize(WindowResizeEvent& e);

	bool OnKeyDown(KeyDownEvent& e);
	bool OnKeyUp(KeyUpEvent& e);

	bool OnMouseMove(MouseMoveEvent& e);
	bool OnMouseButtonDown(MouseButtonDownEvent& e);
	bool OnMouseButtonUp(MouseButtonUpEvent& e);
	bool OnMouseScroll(MouseScrollEvent& e);

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
		LOG_INFO("OpenGL version: {0}", glGetString(GL_VERSION));

	glClearColor(1.0f, 0.8f, 0.8f, 1.0f);
}

Application::~Application()
{
	delete m_Window;
}

void Application::Run()
{
	while (!m_Window->Closed())
	{
		glClear(GL_COLOR_BUFFER_BIT);

		m_Window->Update();
	}
}

void Application::OnEvent(Event& e)
{
	e.Dispatch<WindowCloseEvent>(BIND_FUNCTION(Application::OnWindowClose));
	e.Dispatch<WindowResizeEvent>(BIND_FUNCTION(Application::OnWindowResize));
		
  	e.Dispatch<KeyDownEvent>(BIND_FUNCTION(Application::OnKeyDown));
	e.Dispatch<KeyUpEvent>(BIND_FUNCTION(Application::OnKeyUp));

	e.Dispatch<MouseMoveEvent>(BIND_FUNCTION(Application::OnMouseMove));
	e.Dispatch<MouseButtonDownEvent>(BIND_FUNCTION(Application::OnMouseButtonDown));
	e.Dispatch<MouseButtonUpEvent>(BIND_FUNCTION(Application::OnMouseButtonUp));
	e.Dispatch<MouseScrollEvent>(BIND_FUNCTION(Application::OnMouseScroll));
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	LOG_WARN("Saving data...");
	LOG_ERROR("Exit");

	return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	LOG_TRACE("Window resized: {0}, {1}", e.GetWidth(), e.GetHeight());
	glViewport(0, 0, e.GetWidth(), e.GetHeight());

	return true;
}

bool Application::OnKeyDown(KeyDownEvent& e)
{
	if (!e.IsRepeat())
		LOG_TRACE("{0} key down", e.GetKeyCode());
	else
		LOG_TRACE("{0} key repeated", e.GetKeyCode());

	return true;
}

bool Application::OnKeyUp(KeyUpEvent& e)
{
	LOG_TRACE("{0} key up", e.GetKeyCode());

	return true;
}

bool Application::OnMouseMove(MouseMoveEvent& e)
{
	LOG_TRACE("Mouse move: {0}, {1}", e.GetX(), e.GetY());

	return true;
}

bool Application::OnMouseButtonDown(MouseButtonDownEvent& e)
{
	LOG_TRACE("Mouse button down: {0}", e.GetButton());

	return true;
}

bool Application::OnMouseButtonUp(MouseButtonUpEvent& e)
{
	LOG_TRACE("Mouse button up: {0}", e.GetButton());

	return true;
}

bool Application::OnMouseScroll(MouseScrollEvent& e)
{
	LOG_TRACE("Mouse scrolled: {0}", e.GetYOffset());

	return true;
}

int main()
{
	Application app;
	app.Run();

	return 0;
}
