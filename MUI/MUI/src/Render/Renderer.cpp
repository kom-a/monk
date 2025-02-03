#include "Renderer.h"

#include <windows.h>

#include <string>
#include <iostream>

#include <MOGL/MOGL.h>

#include "Font.h"

namespace mui
{
	uint32_t Renderer::RenderSettings::MaxVertices = 4096;

	static void CheckError(uint32_t object, uint32_t status)
	{
		switch (status)
		{
		case GL_COMPILE_STATUS:
		{
			int success;
			glGetShaderiv(object, status, &success);
			if (!success)
			{
				int length;
				glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
				char* infoLog = (char*)alloca(length);
				glGetShaderInfoLog(object, length, &length, infoLog);
				std::cout << infoLog << std::endl;
			}
		} break;
		case GL_LINK_STATUS:
		{
			int success;
			glGetProgramiv(object, status, &success);
			if (!success)
			{
				int length;
				glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
				char* infoLog = (char*)alloca(length);
				glGetProgramInfoLog(object, length, &length, infoLog);
				std::cout << infoLog << std::endl;
			}
		} break;
		}
	}

	Renderer::Renderer()
	{
		m_Shader			= CreateShader();
		m_VAO				= CreateVertexArrayObject();
		m_VBO				= CreateVertexBufferObject();

		m_Font				= new MFL::Font(g_FontBuffer_small_pixel);
		m_AtlasTextureID	= CreateAtlasTexture();
	}

	Renderer::~Renderer()
	{
		delete m_Font;
	}

	void Renderer::BeginDraw()
	{
		m_Buffer.clear();
	}

	void Renderer::DrawRect(Vec2f position, Vec2f size, Vec4f color)
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		const uint32_t rectVerticesCount = 6;
		if (m_Buffer.size() + rectVerticesCount >= RenderSettings::MaxVertices)
			Flush();

		float z = 0;

		position.X /= 1920;
		position.Y /= 1080;
		size.X /= 1920;
		size.Y /= 1080;


		Vertex v1 = Vertex(Vec3f(position.X			, position.Y			, z), color, Vec2f(-1.0f, -1.0f));
		Vertex v2 = Vertex(Vec3f(position.X + size.X, position.Y			, z), color, Vec2f(-1.0f, -1.0f));
		Vertex v3 = Vertex(Vec3f(position.X + size.X, position.Y + size.Y	, z), color, Vec2f(-1.0f, -1.0f));
		Vertex v4 = Vertex(Vec3f(position.X			, position.Y + size.Y	, z), color, Vec2f(-1.0f, -1.0f));

		m_Buffer.push_back(v1);
		m_Buffer.push_back(v2);
		m_Buffer.push_back(v3);

		m_Buffer.push_back(v1);
		m_Buffer.push_back(v3);
		m_Buffer.push_back(v4);
	}

	void Renderer::DrawString(std::string_view text, Vec2f position, float fontSize, Vec4f color)
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		float offsetX = 0.0f;
		float z = 0.0f;

		for (uint32_t c : text)
		{
			if (m_Buffer.size() + 6 >= RenderSettings::MaxVertices)
				Flush();

			const MFL::GlyphData& glyphData = m_Font->GetGlyphDataByUnicode(c);

			float scale		= m_Font->GetScaleForFontSize(fontSize);
			float width		= glyphData.Width * scale;
			float height	= glyphData.Height * scale;
			float ascender	= glyphData.Ascender * scale;
			float descender = glyphData.Descender * scale;
			float advance	= glyphData.Advance == 0 ? width : glyphData.Advance * scale;

			if (c == ' ')
			{
				offsetX += advance;
				continue;
			}

			Vertex v1 = Vertex(Vec3f(position.X			+ offsetX	, position.Y			, z), color, Vec2f(glyphData.UV_TopLeft.U	, glyphData.UV_TopLeft.V));
			Vertex v2 = Vertex(Vec3f(position.X + width + offsetX	, position.Y			, z), color, Vec2f(glyphData.UV_TopRight.U	, glyphData.UV_TopRight.V));
			Vertex v3 = Vertex(Vec3f(position.X + width + offsetX	, position.Y + height	, z), color, Vec2f(glyphData.UV_BottomRight.U, glyphData.UV_BottomRight.V));
			Vertex v4 = Vertex(Vec3f(position.X			+ offsetX	, position.Y + height	, z), color, Vec2f(glyphData.UV_BottomLeft.U , glyphData.UV_BottomLeft.V));

			m_Buffer.push_back(v1);
			m_Buffer.push_back(v2);
			m_Buffer.push_back(v3);

			m_Buffer.push_back(v1);
			m_Buffer.push_back(v3);
			m_Buffer.push_back(v4);
		}
	}

	void Renderer::EndDraw()
	{
		Flush();
	}

	void Renderer::Flush()
	{
		StoreOpenGLState();

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Buffer.size() * sizeof(Vertex), m_Buffer.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, UV)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glUseProgram(m_Shader);

		glDrawArrays(GL_TRIANGLES, 0, (int)m_Buffer.size());

		m_Buffer.clear();

		RestoreOpenGLState();
	}

	unsigned int Renderer::CreateShader() const
	{
		const std::string vertexSrc = R"(#version 330 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec4 a_Color;
			layout (location = 2) in vec2 a_UV;

			out vec4 v_Color;
			out vec2 v_UV;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0f);
				v_Color = a_Color;
				v_UV = a_UV;
			}
		)";

		const std::string fragmentSrc = R"(#version 330 core
			out vec4 FragColor;

			in vec4 v_Color;
			in vec2 v_UV;

			uniform sampler2D u_Atlas;

			void main()
			{
				FragColor = v_Color;
			}
		)";

		uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
		uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vertexCSrc = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &vertexCSrc, nullptr);
		glCompileShader(vertexShader);
		CheckError(vertexShader, GL_COMPILE_STATUS);

		const char* fragment = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &fragment, nullptr);
		glCompileShader(fragmentShader);
		CheckError(fragmentShader, GL_COMPILE_STATUS);

		unsigned int shader = glCreateProgram();
		glAttachShader(shader, vertexShader);
		glAttachShader(shader, fragmentShader);
		glLinkProgram(shader);
		CheckError(shader, GL_LINK_STATUS);

		return shader;
	}

	uint32_t Renderer::CreateVertexArrayObject() const
	{
		uint32_t vertexArrayObject = 0;
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		return vertexArrayObject;
	}

	uint32_t Renderer::CreateVertexBufferObject() const
	{
		uint32_t vertexBufferObject = 0;
		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		return vertexBufferObject;
	}

	uint32_t Renderer::CreateAtlasTexture() const
	{
		uint32_t textureId;

		glGenTextures(1, &textureId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Font->GetAtlasWidth(), m_Font->GetAtlasHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, m_Font->GetAtlasTextureBuffer());
		glGenerateMipmap(GL_TEXTURE_2D);

		return textureId;
	}

	void Renderer::StoreOpenGLState()
	{
		glGetIntegerv(GL_VIEWPORT, m_OpenGLRestoreState.Viewport.data());
		glGetIntegerv(GL_BLEND_SRC, &m_OpenGLRestoreState.BlendSrc);
		glGetIntegerv(GL_BLEND_DST, &m_OpenGLRestoreState.BlendDst);

		m_OpenGLRestoreState.IsBlendEnabled = glIsEnabled(GL_BLEND);
	}

	void Renderer::RestoreOpenGLState()
	{
		const OpenGLRestoreState& s = m_OpenGLRestoreState;

		glViewport(
			s.Viewport[0],
			s.Viewport[1],
			s.Viewport[2],
			s.Viewport[3]);

		glBlendFunc(s.BlendSrc, s.BlendDst);

		if (s.IsBlendEnabled)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}

}