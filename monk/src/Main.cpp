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
	Render::EnableBlending(true);

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
	std::shared_ptr<const JSONNode> json_ptr = JSON::ParseFile("res/example.json");
	const JSONNode& json = *json_ptr;

	float pi = json["pi"].GetNumber();
	bool happy = json["happy"].GetBoolean();
	const std::string& name = json["name"].GetString();
	auto nothing = json["nothing"];
	float answer_everything = json["answer"]["everything"].GetNumber();
	const JSONList& list = json["list"].GetList();
	const std::string& object_currency = json["object"]["currency"].GetString();
	const float object_value = json["object"]["value"].GetNumber();

	float buffer_data[] = {
		-0.5f, 0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	BufferLayout layout = {
		{ 0, BufferLayout::AttribType::Float3 },
	};

	uint32_t index_data[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexBuffer buffer(buffer_data, sizeof(buffer_data), layout);
	IndexBuffer indices(index_data, sizeof(index_data) / sizeof(index_data[0]));

	while (!m_Window->Closed())
	{
		float deltaTime = timer.Delta();
		m_Window->PollEvents();

		Render::Clear();
		m_Renderer->DrawIndexed(buffer, indices);

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
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	glViewport(0, 0, e.GetWidth(), e.GetHeight());

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
