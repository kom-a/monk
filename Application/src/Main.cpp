#include <Monk/Monk.h>

DEFINE_LOGGER();
IMPL_LOGGER("Application");

static monk::Unique<mwl::Window> InitWindow()
{
	mwl::WindowProps windowProps;
	windowProps.Title = L"Monk";
	windowProps.VSync = true;
	windowProps.OpenGLContextVersion = mwl::OpenGLVersion::OPENGL_4_6;

	monk::Unique<mwl::Window> window = nullptr;
	window.reset(mwl::Create(windowProps));

	return window;
}

static void InitOpenGL()
{
	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		LOG_ERROR("Failed to init OpenGL 4.6");
}

int main()
{
	monk::Unique<mwl::Window> window = InitWindow();
	InitOpenGL();

	monk::Renderer2D renderer;
	monk::Render::EnableBlending();

	while (!window->Closed())
	{
		window->Update();

		monk::Render::SetClearColor(mml::vec4(0.1f, 0.1f, 0.2f, 1.0f));
		monk::Render::Clear();
		monk::Render::Viewport(0, 0, window->GetWidth(), window->GetHeight());

		monk::Ref<monk::OrthographicCamera> camera = monk::CreateRef<monk::OrthographicCamera>(0, window->GetWidth(), window->GetHeight(), 0, -1, 1);
		renderer.Begin(camera);

		renderer.DrawQuad(mml::vec2(100, 100), mml::vec2(250, 120), mml::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		renderer.DrawCircle(mml::vec2(500, 600), 450, mml::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		renderer.End();

		window->SwapBuffers();
	}

	return 0;
}
