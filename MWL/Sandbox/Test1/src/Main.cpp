#include <MWL/MWL.h>

#include <memory>
#include <iostream>
#include <chrono>
#include <Windows.h>


#include <MOGL/MOGL.h>
#include <WangMauna/WangMauna.h>

float mouseX = 0.0f;

void resizeCallback(mwl::WindowResizeEvent e)
{
	std::cout << e.Width << " " << e.Height << std::endl;
}

void mouseMove(mwl::MouseMovedEvent e)
{
	mouseX = e.X;

	std::cout << "Mouse " <<  e.X << " " << e.Y << std::endl;
}

void mouseButtonDown(mwl::MouseButtonDownEvent e)
{
	std::cout << "Mouse button down at (" << e.X << ", " << e.Y << ")\t button: " << (int)e.Button << std::endl;;
}

void mouseButtonUp(mwl::MouseButtonUpEvent e)
{
	std::cout << "Mouse button up at (" << e.X << ", " << e.Y << ")\t button: " << (int)e.Button << std::endl;;
}

void mouseClicked(mwl::MouseClickedEvent e)
{
	std::cout << "Mouse clicked at (" << e.X << ", " << e.Y << ")\t button: " << (int)e.Button << std::endl;
}

void mouseScroll(mwl::MouseScrollEvent e)
{
	std::cout << "Scroll: " << e.Delta << std::endl;
}

void keyDown(mwl::KeyDownEvent e)
{
	if (e.Repeat) 
		return;

	std::cout << "Key down " << (char)e.Key << "   Repeat: " << e.Repeat << std::endl;
}

void keyUp(mwl::KeyUpEvent e)
{
	std::cout << "Key up " << (char)e.Key << std::endl;
}

int main()
{
	mwl::WindowProps props;
	props.Closed = false;
	props.VSync = true;
	props.Width = 1600;
	props.Height = 900;
	props.Title = L"Hello";
	props.OpenGLContextVersion = mwl::OpenGLVersion::OPENGL_4_6;
	mwl::Window* window = mwl::Create(props);
	//window->SetWindowResizeCallback(resizeCallback);
	window->SetMouseMovedCallback(mouseMove);
	//window->SetMouseButtonDownCallback(mouseButtonDown);
	//window->SetMouseButtonUpCallback(mouseButtonUp);
	//window->SetMouseClickedCallback(mouseClicked);
	//window->SetMouseScrollCalback(mouseScroll);
	window->SetKeyDownCallback(keyDown);
//	window->SetKeyUpCallback(keyUp);

	//mwl::Cursor cursor(L"res/oxy-bluecurve/oxy-bluecurve.inf");
	//window->SetCursor(cursor);

	if (mogl::OpenGLVersion::OPENGL_4_6 != mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6))
	{
		std::cout << "Failed to load opengl" << std::endl;
	}

	const char* version = (const char*)glGetString(GL_VERSION);

	std::cout << version << std::endl;

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data[] = {
	   -0.5f, -0.5f, 0.0f,
	    0.5f, -0.5f, 0.0f,
	    0.0f,  0.5f, 0.0f,
	};

	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glBufferData(123123, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	while (!window->Closed())
	{
		window->Update();

		if (mouseX > 500)
		{
			//PostMessageW((HWND)window->GetNative(), WM_SETCURSOR, 0, HTLEFT);
		}

		glViewport(0, 0, window->GetWidth(), window->GetHeight());

		glClearColor(0.9f, 0.7f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		window->SwapBuffers();
	}

	return 0;
}
