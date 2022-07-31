#include <iostream>
#include <chrono>

#include "core/Window.h"
#include "core/Log.h"

#include <gl/glew.h>

int main()
{
	using namespace monk;

	Window window(1280, 720, "Monk");
	window.SwapInterval(1);

	unsigned vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	unsigned vertex_object;
	glGenBuffers(1, &vertex_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_object);

	float buffer[] = {
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

	auto lastTime = std::chrono::system_clock::now();

	while (!window.Closed())
	{
		auto currentTime = std::chrono::system_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
		lastTime = currentTime;

		LOG_INFO("delta: {0}", delta);

		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		window.Update();
	}

	glDeleteBuffers(1, &vertex_object);
	glDeleteVertexArrays(1, &vertex_array);

	return 0;
}
