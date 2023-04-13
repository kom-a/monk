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
	bool OnButtonDown(MouseButtonDownEvent& e);
	bool OnButtonUp(MouseButtonUpEvent& e);
	void Update(float deltaTime);

private:
	Window* m_Window;
	Renderer2D* m_Renderer2D;
	Renderer* m_Renderer;
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

	Render::SetClearColor(math::vec4(0.2f, 0.2f, 0.25f, 1.0f));
	Render::EnableDepthTest(true);
	Render::EnableBlending(true);
	Render::EnableCulling(true);

	m_Renderer2D = new Renderer2D();
	m_Renderer = new Renderer();

	Random::Seed(Time::CurrentTime());
}

Application::~Application()
{
	delete m_Window;
	delete m_Renderer2D;
	delete m_Renderer;
}

void Application::Run()
{
	Time timer;

	Shared<Camera> camera = std::make_shared<Camera>(45.0f, (float)m_Window->GetWidth() / m_Window->GetHeight(), 0.01f, 100.0f);
	camera->SetPosition(math::vec3(0.0f, 0.0f, -10.0f));
	camera->SetTarget(math::vec3(0.0f));
	CameraController cameraController(camera);

	Shared<Model> model = ModelLoader::LoadFromFile("res/models/this_tree_is_growing/scene.gltf");
	
	auto vertex_src = FileManager::ReadFile("res/GLTFShader.vert");
	auto fragment_src = FileManager::ReadFile("res/GLTFShader.frag");
	Shared<Shader> gltfShader = std::make_shared<Shader>(vertex_src, fragment_src);

	Renderer renderer;

	while (!m_Window->Closed())
	{
		float deltaTime = timer.Delta();
		m_Window->PollEvents();
		cameraController.Update(deltaTime);

		Render::Clear();

		renderer.Begin(camera, gltfShader);

		renderer.DrawModel(model);

		renderer.End();

		Update(deltaTime);

		if (Input::IsKeyPressed(Key::Escape))
			m_Window->Close();

		m_Window->Update();
		Input::Update();
	}
}

void Application::OnEvent(Event& e)
{
	e.Dispatch<WindowResizeEvent>(BIND_FUNCTION(Application::OnWindowResize));
	e.Dispatch<MouseButtonDownEvent>(BIND_FUNCTION(Application::OnButtonDown));
	e.Dispatch<MouseButtonUpEvent>(BIND_FUNCTION(Application::OnButtonUp));
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	glViewport(0, 0, e.GetWidth(), e.GetHeight());

	return true;
}

bool Application::OnButtonDown(MouseButtonDownEvent& e)
{
	if (e.GetButton() == Mouse::ButtonRight)
	{
		m_Window->HideCursor(true);
		m_Window->LockCursor(true);
	}

	return true;
}

bool Application::OnButtonUp(MouseButtonUpEvent& e)
{
	if (e.GetButton() == Mouse::ButtonRight)
	{
		m_Window->HideCursor(false);
		m_Window->LockCursor(false);
	}

	return true;
}

void Application::Update(float deltaTime)
{

}

int main()
{
	Application app;
	app.Run();

	return 0;
}
