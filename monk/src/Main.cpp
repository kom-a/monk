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
	std::shared_ptr<const JSONNode> json_ptr = JSON::ParseFile("res/models/Box/Box.gltf");
	const JSONNode& json = *json_ptr;

	struct
	{
		uint32_t ByteLength;
		std::string Uri;
	} gltfBuffer{ 0 };

	gltfBuffer.ByteLength = json["buffers"].GetList()[0]->GetJSONObject().at("byteLength")->GetNumber();
	gltfBuffer.Uri = json["buffers"][0]["uri"].GetString();

	FileData gltfBin = FileManager::ReadBytes(std::string("res/models/Box/") + gltfBuffer.Uri);
	MONK_ASSERT(gltfBin.Size == gltfBuffer.ByteLength);

	unsigned vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 72, gltfBin.Data + 576, GL_STATIC_DRAW);
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 576, gltfBin.Data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12, (const void*)(288));
	glEnableVertexAttribArray(1);

	std::string vertex_src = FileManager::ReadFile("res/Renderer.vert");
	std::string fragment_src = FileManager::ReadFile("res/Renderer.frag");
	Shader shader(vertex_src, fragment_src);

	math::mat4 projection = math::Perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);

	Camera camera(45.0f, (float)m_Window->GetWidth() / m_Window->GetHeight(), 0.01f, 100.0f);
	camera.SetPosition(math::vec3(0.0f, 0.0f, -10.0f));
	camera.SetTarget(math::vec3(0.0f));
	CameraController controller(&camera);
	
	float cameraRadius = 5;
	math::vec2 cameraRotation(0.0f);

	math::vec3 direction(0.0f, 0.0f, -1.0f);

	Shared<Model> model = ModelLoader::LoadFromFile("res/models/this_tree_is_growing/scene.gltf");
	
	vertex_src = FileManager::ReadFile("res/GLTFShader.vert");
	fragment_src = FileManager::ReadFile("res/GLTFShader.frag");
	Shader gltfShader(vertex_src, fragment_src);

	while (!m_Window->Closed())
	{
		float deltaTime = timer.Delta();
		m_Window->PollEvents();
		controller.Update(deltaTime);

		Render::Clear();

		shader.Bind();
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		shader.SetMatrix4("u_ProjectionView", camera.GetProjectionViewMatrix());

		// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

		gltfShader.Bind();
		gltfShader.SetMatrix4("u_ProjectionView", camera.GetProjectionViewMatrix());

		for (auto mesh : model->m_Meshes)
		{
			mesh.m_VertexBuffer->Bind();
			mesh.m_IndexBuffer->Bind();
			gltfShader.SetMatrix4("u_Model", mesh.m_ModelMatrix);
			glDrawElements(GL_TRIANGLES, mesh.m_IndexBuffer->Count(), IndexBuffer::ToOpenGLType(mesh.m_IndexBuffer->Type()), nullptr);
		}

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
