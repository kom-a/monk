#include <iostream>
#include <functional>
#include <string>
#include <chrono>

#include <MWL/MWL.h>
#include <MOGL/MOGL.h>
#include <MMath/MMath.h>

#include "core/Memory.h"
#include "core/Log.h"
#include "core/Random.h"
#include "graphics/Renderer2D.h"
#include "core/Time.h"

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

using namespace monk;

class Application
{
public:
	Application();
	~Application() = default;

	void Run();

private:
	void InitWindow();
	void LoadOpenGL();
	void Update(float deltaTime);

	void OnWindowResize(mwl::WindowResizeEvent e);

private:
	monk::Unique<mwl::Window> m_Window;
	monk::Unique<monk::Renderer2D> m_Renderer2D;
};

Application::Application()
{
	InitWindow();
	LoadOpenGL();

	m_Renderer2D = monk::CreateUnique<Renderer2D>();
}

void Application::Run()
{
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

	monk::OrthographicCamera camera(0, 1, 1, 0, 0, 1);

	monk::Time timer;

	while (!m_Window->Closed())
	{
		glClear(GL_COLOR_BUFFER_BIT);

		m_Renderer2D->Begin(camera);

		float k = 5.0f;

		float step = 1.0f / k;

		for (float i = 0; i < 1; i += step)
		{
			for (float j = 0; j < 1; j += step)
			{
				m_Renderer2D->DrawQuad(mmath::vec2(i, j), mmath::vec2(step), mmath::vec4(i, j, std::sin(timer.Elapsed()) * 0.5f + 0.5f, 1.0f));
			}
		}

		m_Renderer2D->End();

		m_Window->Update();
	}
}

void Application::InitWindow()
{
	mwl::SetOpenGLVersion(mwl::OpenGLVersion::OPENGL_4_6);

	auto windowProps = mwl::WindowProps();
	windowProps.Title = L"Monk";

	m_Window.reset(mwl::Create(windowProps));

	m_Window->SetWindowResizeCallback(BIND_FUNCTION(Application::OnWindowResize));

	m_Window->SetCursor(mwl::Cursor(L"res/oxy-bluecurve/oxy-bluecurve.inf"));
}

void Application::LoadOpenGL()
{
	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		LOG_ERROR("Failed to load opengl");
	else
		LOG_INFO("\nOpenGL: \n\tvendor: {0}\n\trenderer: {1}\n\tversion: {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
}

void Application::OnWindowResize(mwl::WindowResizeEvent e)
{
	glViewport(0, 0, e.Width, e.Height);
}

int main()
{
	Application app;
	app.Run();

	return 0;
}
