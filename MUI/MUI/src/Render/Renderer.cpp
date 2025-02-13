#include "Renderer.h"

#include <windows.h>

#include <string>
#include <iostream>

#include <MOGL/MOGL.h>

#include "FontAtlas.h"

namespace mui
{
	Renderer::Renderer()
	{
		const std::string vertexSrc = R"(#version 330 core
			layout (location = 0) in vec2 a_Position;
			layout (location = 1) in vec4 a_Color;
			layout (location = 2) in vec2 a_UV;

			layout (location = 3) in vec2 a_Center;
			layout (location = 4) in float a_Radius;

			out vec2 v_Position;
			out vec4 v_Color;
			out vec2 v_UV;

			out vec2 v_Center;
			out float v_Radius;

			uniform mat4 u_ProjectionMatrix;

			void main()
			{
				gl_Position = u_ProjectionMatrix * vec4(a_Position, 0.0f, 1.0f);

				v_Position	= a_Position;
				v_Color		= a_Color;
				v_UV		= a_UV;

				v_Center	= a_Center;
				v_Radius	= a_Radius;
			}
		)";

		const std::string fragmentSrc = R"(#version 330 core
			out vec4 FragColor;

			in vec2 v_Position;
			in vec4 v_Color;
			in vec2 v_UV;

			in vec2 v_Center;
			in float v_Radius;

			uniform sampler2D u_Atlas;

			vec4 GetAtlasColor(float alpha)
			{
				return vec4(alpha, alpha, alpha, alpha);
			}

			void main()
			{
				if(v_Radius != -1.0f)
				{
					float dist = length(v_Position - v_Center);
					const float eps = 0.5f;
					FragColor = vec4(v_Color.xyz, 1.0f - smoothstep(v_Radius - eps, v_Radius + eps, dist));
				}
				else
				{
					if(v_UV == vec2(-1.0f, -1.0f))
						FragColor = v_Color; 
					else 
						FragColor = GetAtlasColor(texture(u_Atlas, v_UV).r) * v_Color;
				}
			}
		)";

		m_VertexBuffer = std::make_unique<VertexBuffer>(DrawList::MaxQuadCount());
		m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);

		//m_Font			= std::make_unique<MFL::Font>("C:/Windows/Fonts/arial.ttf");
		//m_Font			= std::make_unique<MFL::Font>("C:/Users/kamil/OneDrive/Рабочий стол/Roboto-Medium.ttf");
		m_Font = std::make_unique<MFL::Font>(g_FontBuffer_small_pixel);
		m_AtlasTexture	= CreateAtlasTexture();
	}

	void Renderer::SetViewport(float x, float y, float width, float height)
	{
		m_Viewport.X = x;
		m_Viewport.Y = y;
		m_Viewport.Width = width;
		m_Viewport.Height = height;
	}

	void Renderer::SetWindowViewport(float x, float y, float width, float height)
	{
		m_WindowViewport.X = x;
		m_WindowViewport.Y = y;
		m_WindowViewport.Width = width;
		m_WindowViewport.Height = height;
	}

	void Renderer::FlushDrawList(const DrawList& drawList)
	{
		StoreOpenGLState();

		float L = m_WindowViewport.X;
		float R = L + m_WindowViewport.Width;
		float T = m_WindowViewport.Y;
		float B = T + m_WindowViewport.Height;

		std::array<float, 16> projectionMatrix = {
			2.0f / (R - L)		, 0.0f				, 0.0f	, 0.0f,
			0.0f				, 2.0f / (T - B)	, 0.0f	, 0.0f,
			0.0f				, 0.0f				, -1.0f	, 0.0f,
			(R + L) / (L - R)	, (T + B) / (B - T)	, 0.0f	, 1.0f
		};

		m_Shader->Bind();
		m_Shader->SetMatrix4("u_ProjectionMatrix", projectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AtlasTexture);
		m_Shader->SetInt("u_Atlas", 0);

		//glViewport(x, sh - y - h, w, h);

		glViewport(m_WindowViewport.X, m_Viewport.Height - m_WindowViewport.Y - m_WindowViewport.Height, m_WindowViewport.Width, m_WindowViewport.Height);

		for (const DrawListBuffer& dl : drawList.m_DrawList)
		{
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			m_VertexBuffer->SetData(dl.Buffer.data(), dl.BufferStats.VerticesCount);

			glDrawElements(GL_TRIANGLES, dl.BufferStats.IndicesCount, GL_UNSIGNED_INT, nullptr);
		}

		RestoreOpenGLState();
	}

	Vec2f Renderer::CalcTextSize(std::string_view text, float fontSize)
	{
		MFL::VerticalMetrics vm = m_Font->GetVerticalMetrics();
		float scale = m_Font->GetScaleForFontSize(fontSize);

		Vec2f size{ 0.0f, vm.LineHeight * scale };

		float x = 0.0f;

		for (uint32_t c : text)
		{
			const MFL::GlyphData& glyphData = m_Font->GetGlyphDataByUnicode(c);
			float width = glyphData.Width * scale;
			float height = glyphData.Height * scale;
			float ascender = glyphData.Ascender * scale;
			float descender = glyphData.Descender * scale;
			float advance = glyphData.Advance == 0 ? width : glyphData.Advance * scale;

			if (c == '\n')
			{
				size.X = max(size.X, x);
				size.Y += (vm.LineGap + vm.LineHeight) * scale;
				x = 0;
			}

			x += advance;
		}

		size.X = max(size.X, x);

		return size;
	}

	MFL::Font* Renderer::GetFont()
	{
		return m_Font.get();
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
		m_OpenGLRestoreState.IsBlendEnabled = glIsEnabled(GL_DEPTH_TEST);
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

		if (s.IsDepthTestEnabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
}