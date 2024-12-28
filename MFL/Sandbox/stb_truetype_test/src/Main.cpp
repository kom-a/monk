#include <iostream>

#include <MFL/MFL.h>
#include <MWL/MWL.h>
#include <MOGL/MOGL.h>

#include "Shader.h"
#include "stb_truetype.h"

uint32_t InitVAO()
{
	uint32_t VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	return VAO;
}

uint32_t InitVBO()
{
	uint32_t VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	float vertices[] = { 
		-1.0f,  1.0f, 0.0f,		0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,		1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 1.0f,

		-1.0f,  1.0f, 0.0f,		0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	return VBO;
}

const char* const g_VertexShaderSource = R"(
	#version 430 core

	layout(location = 0) in vec3 a_pos;
	layout(location = 1) in vec2 a_tex;

	out vec2 v_tex;

	void main()
	{
		gl_Position = vec4(a_pos, 1.0f);
		
		v_tex = a_tex;
	}
)";

const char* const g_FragmentShaderSource = R"(
	#version 430 core

	out vec4 FragColor;

	in vec2 v_tex;

	uniform sampler2D u_texture;

	void main()
	{
		FragColor = vec4(texture(u_texture, v_tex).r);
	}
)";

int main()
{
	mwl::SetOpenGLVersion(mwl::OpenGLVersion::OPENGL_4_6);
	mwl::Window* window = mwl::Create();
	window->SetFullscreen(true);

	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		std::cout << "Failed to load opengl" << std::endl;

	uint8_t* ttfBuffer = new uint8_t[1 << 20];
	uint8_t* tmpBitmap = new uint8_t[1024 * 1024]; //res of the bitmap

	stbtt_bakedchar cdata[256];

	fread(ttfBuffer, 1, 1 << 20, fopen("C:/Windows/Fonts/Carlito-Regular.ttf", "rb")); //rb makes and writes into a binary file

	stbtt_BakeFontBitmap(ttfBuffer, 0, 128.0f, tmpBitmap, 1024, 1024, 32, 256, cdata);

	uint32_t VAO = InitVAO();
	uint32_t VBO = InitVBO();

	uint32_t textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1024, 1024, 0, GL_RED, GL_UNSIGNED_BYTE, tmpBitmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	monk::Shader shader(g_VertexShaderSource, g_FragmentShaderSource);

	while (!window->Closed())
	{
		window->Update();
		glViewport(0, 0, window->GetWidth(), window->GetHeight());

		shader.Bind();
		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		window->SwapBuffers();
	}


	return 0;
}