#include <iostream>
#include <functional>
#include <string>
#include <chrono>

#include "Monk.h"

#include <windows.h>

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
	bool OnWindowResize(WindowResizeEvent& e);

private:
	Window* m_Window;
	Renderer2D* m_Renderer;
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

	m_Renderer = new Renderer2D();
	m_Renderer->SetClearColor(math::vec4(0.3f, 0.4f, 0.8f, 1.0f));
	m_Renderer->EnableAlphaBlending(true);

	Random::Seed(Time::CurrentTime());
}

Application::~Application()
{
	delete m_Window;
	delete m_Renderer;
}

void Application::Run()
{
	Time timer;
	OrthographicCamera camera(math::Ortho(0, m_Window->GetWidth(), m_Window->GetHeight(), 0, -1, 1));
	
	math::vec2 grid = { 8, 8 };
	int fps = 0;
	float lastFpsTime = 0;

	Ref<Texture2D> textures[32]; 

	// There is a memory leak here :(
	textures[0] = Texture2D::Create("res/images/cubes.pam");
	textures[1] = Texture2D::Create("res/images/monk2.ppm");
	textures[2] = Texture2D::Create("res/images/monk1.ppm");

	bool showGui = false;

	math::vec3 cameraPosition = math::vec3(0.0f);
	
	while (!m_Window->Closed())
	{
		m_Window->PollEvents();

		float deltaTime = timer.Delta();
		if (timer.Elapsed() - lastFpsTime > 1)
		{
			lastFpsTime = timer.Elapsed();
			LOG_INFO("FPS: {0}", fps);
			fps = 0;
		}
		fps++;

		if (Input::IsKeyUp(Key::G))
			showGui = !showGui;

		const float cameraSpeed = 1;
		if (Input::IsKeyPressed(Key::W))
			cameraPosition.y += cameraSpeed * deltaTime;
		if(Input::IsKeyPressed(Key::S))
			cameraPosition.y -= cameraSpeed * deltaTime;
		if (Input::IsKeyPressed(Key::A))
			cameraPosition.x -= cameraSpeed * deltaTime;
		if (Input::IsKeyPressed(Key::D))
			cameraPosition.x += cameraSpeed * deltaTime;
		camera.SetPosition(cameraPosition);

		m_Renderer->Begin(camera);
		m_Renderer->Clear();

		math::vec2 cell = { (float)m_Window->GetWidth() / grid.x, (float)m_Window->GetHeight() / grid.y };

		for (int y = 1; y < grid.y - 1; y++)
		{
			for (int x = 1; x < grid.x - 1; x++)
			{
				if((x + y) % 3 == 0) 
					m_Renderer->DrawTexture(math::vec2(x * cell.x, y * cell.y), cell, *textures[0]);
				else if ((x + y) % 3 == 1)
					m_Renderer->DrawTexture(math::vec2(x * cell.x, y * cell.y), cell, *textures[1]);
				else
					m_Renderer->DrawTexture(math::vec2(x * cell.x, y * cell.y), cell, *textures[2]);
			}
		}

		m_Renderer->End();

		if (Input::IsKeyPressed(Key::Escape))
			m_Window->Close();

		m_Window->Update();
		Input::Update();
	}
}

void Application::OnEvent(Event& e)
{
	e.Dispatch<WindowResizeEvent>(BIND_FUNCTION(Application::OnWindowResize));
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	
	return true;
}

int main()
{
	Application app;
	app.Run();

	return 0;
}
