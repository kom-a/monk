#pragma once

#include <vector>
#include<array>

#include <MFL/MFL.h>

#include "Math.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "DrawList.h"

namespace mui
{
	struct OpenGLRestoreState
	{
		uint32_t VAO = 0;
		uint32_t VBO = 0;
		uint32_t ShaderID = 0;

		std::array<int, 4> Viewport;

		bool IsBlendEnabled;
		bool IsDepthTestEnabled;
		int BlendSrc;
		int BlendDst;
	};

	struct Viewport
	{
		float X;
		float Y;
		float Width;
		float Height;
	};

	class Renderer
	{
	public:
		Renderer();

		void SetWindowViewport(float x, float y, float width, float height);
		void SetViewport(float x, float y, float width, float height);

		void FlushDrawList(const DrawList& drawList);

		Vec2f CalcTextSize(std::string_view text, float fontSize);

		MFL::Font* GetFont();

	private:
		uint32_t CreateAtlasTexture() const;

		void StoreOpenGLState();
		void RestoreOpenGLState();

	private:
		OpenGLRestoreState m_OpenGLRestoreState;

		Viewport m_Viewport = { 0.0f, 0.0f, 0.0f, 0.0f };
		Viewport m_WindowViewport = { 0.0f, 0.0f, 0.0f, 0.0f };

		std::unique_ptr<VertexBuffer>	m_VertexBuffer	= nullptr;
		std::unique_ptr<MFL::Font>		m_Font			= nullptr;
		std::unique_ptr<Shader>			m_Shader		= nullptr;
		uint32_t						m_AtlasTexture	= (uint32_t)-1;
	};
}