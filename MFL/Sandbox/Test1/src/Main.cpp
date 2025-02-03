#include <MOGL/MOGL.h>
#include <MWL/MWL.h>
#include <Monk/Monk.h>
#include <MFL/MFL.h>

#include <iostream>
#include <assert.h>

mwl::Window* window = nullptr;

void OnKeyDown(const mwl::KeyDownEvent& e)
{
	if (e.Key == mwl::KeyCode::F11)
		window->SetFullscreen(!window->IsFullscreen());
}

int main()
{
	mwl::WindowProps windowProps;
	windowProps.OpenGLContextVersion = mwl::OpenGLVersion::OPENGL_4_6;
	window = mwl::Create(windowProps);
	window->SetCursor(mwl::Cursor("./oxy-bluecurve/oxy-bluecurve.inf"));
	window->SetKeyDownCallback(OnKeyDown);

	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		assert(false && "failed to load opengl");

	monk::Renderer2D renderer;
	monk::Render::EnableBlending(true);
	monk::Render::EnableDepthTest(true);

	monk::Ref<monk::OrthographicCamera> camera = monk::CreateRef<monk::OrthographicCamera>(0, window->GetWidth(), window->GetHeight(), 0, -1, 1);

	monk::Time timer;

	float y = 0;

	std::string fox = "The quick brown fox jumps over the lazy dog!";

	MFL::Font arial = MFL::Font("C:/Users/kamil/OneDrive/Рабочий стол/ponde___.ttf");
	monk::Ref<monk::Texture2D> atlasTexture = monk::Texture2D::Create(arial.GetAtlasWidth(), arial.GetAtlasHeight(), monk::TextureFormat::RED, arial.GetAtlasTextureBuffer());

	while (!window->Closed())
	{
		window->Update();

		monk::Render::SetClearColor(0.65f, 0.05f, 0.05f, 1.0f);
		monk::Render::Clear();
		monk::Render::Viewport(window->GetWidth(), window->GetHeight());

		camera->SetProjection(0, window->GetWidth(), window->GetHeight(), 0, -1, 1);

		renderer.Begin(camera);

		//renderer.DrawString(text, mml::vec2(10, y), 12, mml::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		//renderer.DrawTexture(mml::vec2(0.0f, 0.0f), atlasTexture->Size() / 2, atlasTexture);

		renderer.DrawQuad(mml::vec2(0.0f, 350), mml::vec2(window->GetWidth(), 1000), mml::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		renderer.DrawString(fox, mml::vec2(100, 100), 10, mml::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//renderer.DrawString(fox, mml::vec2(100, 150), 18, mml::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//renderer.DrawString(fox, mml::vec2(100, 200), 24, mml::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//renderer.DrawString(fox, mml::vec2(100, 250), 32, mml::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//renderer.DrawString(fox, mml::vec2(100, 300), 36, mml::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		renderer.DrawString(fox, mml::vec2(100, 350), 42, mml::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//renderer.DrawString(text, mml::vec2(100, 400), 46, mml::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//renderer.DrawString(fox, mml::vec2(100, 500), 64, mml::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//renderer.DrawString(fox, mml::vec2(100, 600), 88, mml::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		renderer.End();

		y = -timer.Elapsed() * 50;

		if (y <= -1000)
		{
			timer.Reset();
		}

		window->SwapBuffers();
	}

	return 0;
}