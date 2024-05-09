#include <iostream>

#include <WangMauna/WangMauna.h>
#include <MWL/MWL.h>
#include <MOGL/MOGL.h>
#include <MUI/MUI.h>

DEFINE_LOGGER();
IMPL_LOGGER("Mui Sandbox");

int main()
{
	mwl::Window* window = mwl::Create();

	mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_3_3);

	using namespace mui;

	Mui::Init();

	glClearColor(0.5f, 0.6f, 0.8f, 1.0f);
	while (!window->Closed())
	{
		glClear(GL_COLOR_BUFFER_BIT);

		Mui::NewFrame(mui::Viewport(0, 0, window->GetWidth(), window->GetHeight()));

		Mui::Begin("Hello world");

		if (Mui::Button("Click me"))
		{
			LOG_INFO("Button clicked");
		}
		Mui::End();

		Mui::EndFrame();

		window->Update();

	}


	return 0;
}