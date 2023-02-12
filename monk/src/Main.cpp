#include <iostream>
#include <functional>
#include <string>
#include <chrono>

#include "Monk.h"

#include <windows.h>

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

using namespace monk;

math::mat4 lookAt(const math::vec3 position, const math::vec3& target)
{
	math::vec3 cameraDirection = math::Normalize(target - position);
	math::vec3 up = math::vec3(0.0f, 1.0f, 0.0f);
	math::vec3 cameraRight = math::Normalize(math::Cross(up, cameraDirection));
	math::vec3 cameraUp = math::Cross(cameraDirection, cameraRight);

	const math::vec3& R = cameraRight;
	const math::vec3& U = cameraUp;
	const math::vec3& D = cameraDirection;
	const math::vec3& P = position;

	math::mat4 left(
		R.x, R.y, R.z, 0,
		U.x, U.y, U.z, 0,
		D.x, D.y, D.z, 0,
		0, 0, 0, 1);

	math::mat4 right(
		1, 0, 0, -P.x,
		0, 1, 0, -P.y,
		0, 0, 1, -P.z,
		0, 0, 0, 1);

	return left * right;
}

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
	Render::EnableDepthTest(true);
	Render::EnableBlending(true);


	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);*/

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
	} gltfBuffer { 0 };
	
	gltfBuffer.ByteLength = json["buffers"].GetList()[0]->GetJSONObject().at("byteLength")->GetNumber();
	gltfBuffer.Uri = json["buffers"][0]["uri"].GetString();

	FileData gltfBin = FileManager::ReadBytes(std::string("res/models/Box/") + gltfBuffer.Uri);
	MONK_ASSERT(gltfBin.Size == gltfBuffer.ByteLength);

	unsigned vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const uint32_t count = 36;
	uint8_t* indices_buffer = new uint8_t[sizeof(uint16_t) * count];
	memcpy(indices_buffer, gltfBin.Data + 576, 72);

	/*uint16_t indices_buffer[] = {
		0, 1, 2,
		2, 3, 0
	};*/
	
	unsigned ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * count, indices_buffer, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_buffer), indices_buffer, GL_STATIC_DRAW);

	uint32_t accessor_byte_offset = 288;
	uint32_t accessor_count = 24;

	uint8_t* vertex_data = new uint8_t[24 * sizeof(float) * 3];
	memcpy(vertex_data, gltfBin.Data + 288, 24 * sizeof(float) * 3);

	/*float vertex_data[] = {
		-0.5f, 0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};*/

	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float) * 3, vertex_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	std::string vertex_src = FileManager::ReadFile("res/Renderer.vert");
	std::string fragment_src = FileManager::ReadFile("res/Renderer.frag");
	Shader shader(vertex_src, fragment_src);

	//math::mat4 projection = math::Ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	math::mat4 projection = math::Perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
	
	math::vec3 position(0.0f, 0.0f, -4.0f);
	math::mat4 view = lookAt(position, math::vec3(0.0f));
	view = math::mat4(1.0f);
	view.Translate(math::vec3(0.0f, 0.0f, -10));

	while (!m_Window->Closed())
	{
		float deltaTime = timer.Delta();
		m_Window->PollEvents();

		Render::Clear();
		
		const float cameraSpeed = 5.0f;
		if (Input::IsKeyPressed(Key::D))
			position.x += cameraSpeed * deltaTime;
		if (Input::IsKeyPressed(Key::A))
			position.x -= cameraSpeed * deltaTime;
		if (Input::IsKeyPressed(Key::W))
			position.y += cameraSpeed * deltaTime;
		if (Input::IsKeyPressed(Key::S))
			position.y -= cameraSpeed * deltaTime;
		
		shader.Bind();
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		math::mat4 view = lookAt(position, math::vec3(0.0f));
		shader.SetMatrix4("u_Projection", projection);
		shader.SetMatrix4("u_View", view);

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

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
