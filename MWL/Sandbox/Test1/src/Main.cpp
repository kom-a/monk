#include <MWL/MWL.h>

#include <memory>
#include <iostream>
#include <chrono>
#include <Windows.h>


#include <MOGL/MOGL.h>
#include <WangMauna/WangMauna.h>

int main()
{
	mwl::SetOpenGLVersion(mwl::OpenGLVersion::OPENGL_4_6);
	mwl::Window* window = mwl::Create();

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

		window->Update();
	}

	return 0;
}
