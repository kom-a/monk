#pragma once

#include <MML/MML.h>
#include <MFL/MFL.h>

#include "../core/Memory.h"

#include "Buffer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "OrthographicCamera.h"

namespace monk
{
	class Renderer2D
	{
	public:
		Renderer2D();

	public:
		struct QuadVertex
		{
			mml::vec3 Position;
			mml::vec4 Color;
			float TextureID;
			mml::vec2 TextureCoords;

			static BufferLayout GetLayout()
			{
				return {
					{ 0, BufferLayout::AttribType::Float3 },
					{ 1, BufferLayout::AttribType::Float4 },
					{ 2, BufferLayout::AttribType::Float  },
					{ 3, BufferLayout::AttribType::Float2 }
				};
			}
		};

		struct CircleVertex
		{
			mml::vec3 Position;
			mml::vec3 Center;
			mml::vec4 Color;
			float Radius;

			static BufferLayout GetLayout()
			{
				return {
					{ 0, BufferLayout::AttribType::Float3 },
					{ 1, BufferLayout::AttribType::Float3 },
					{ 2, BufferLayout::AttribType::Float4 },
					{ 3, BufferLayout::AttribType::Float }
				};
			}
		};

		struct TextVertex
		{
			mml::vec3 Position;
			mml::vec4 Color;
			mml::vec2 TextureCoords;
			float AtlasIndex;

			static BufferLayout GetLayout()
			{
				return {
					{ 0, BufferLayout::AttribType::Float3 },
					{ 1, BufferLayout::AttribType::Float4 },
					{ 2, BufferLayout::AttribType::Float2 },
					{ 3, BufferLayout::AttribType::Float  }
				};
			}
		};

	public:
		void Begin(Ref<const OrthographicCamera> camera);
		void End();

		void DrawQuad(const mml::vec2& position, const mml::vec2& size, const mml::vec4& color);
		void DrawTexture(const mml::vec2& position, const mml::vec2& size, Ref<Texture2D> texture);

		void DrawRect(const mml::vec2& position, const mml::vec2& size, const mml::vec4& color);
		
		void DrawCircle(const mml::vec2& center, float radius, const mml::vec4& color);

		void DrawString(const std::string& text, const mml::vec2& position, float fontSize, const mml::vec4& color = mml::vec4(1.0f));

		void DrawLine(const mml::vec2& p0, const mml::vec2& p1, const mml::vec4& color, float thickness = 1.0f);
		void DrawBezierCurve(const mml::vec2& p0, const mml::vec2& p1, const mml::vec2& controlPoint, const mml::vec4& color, float thickness = 2.0f, uint32_t segments = 64);

		void PushFont(Ref<MFL::Font> font);
		void PopFont();

	private:
		void InitQuads();
		void InitCircles();
		void InitText();

		void BeginQuadsBatch();
		void EndQuadsBatch();

		void BeginCirclesBatch();
		void EndCirclesBatch();

		void BeginTextBatch();
		void EndTextBatch();

		void Flush();
		void FlushQuads();
		void FlushCircles();
		void FlushText();

		void NextQuadBatch();
		void NextCircleBatch();

		float GetBatchTextureID(Ref<Texture2D> texture);

	private:
		const struct
		{
			const size_t MaxQuads = 1000;
			const size_t MaxTextures = 16;
		} QuadBatchSettings;

		const struct
		{
			const size_t MaxCircles = 500;
		} CircleBatchSettings;

		const struct
		{
			const size_t MaxFonts = 12;
			const size_t MaxChars = 1 << 16;
		} TextBatchSettings;

		struct QuadBatchStats
		{
			size_t Quads		= 0;
			size_t Verticies	= 0;
			size_t Indices		= 0;
			std::vector<Ref<Texture2D>> Textures;

			void Reset()
			{
				Quads = 0;
				Verticies = 0;
				Indices = 0;
				for (auto texture : Textures)
				{
					texture.reset();
				}
				Textures.clear();
			}
		};

		struct CircleBatchStats
		{
			size_t Circles = 0;
			size_t Verticies = 0;
			size_t Indices = 0;

			void Reset()
			{
				Circles = 0;
				Verticies = 0;
				Indices = 0;
			}
		};

		struct TextBatchStats
		{
			size_t Characters	= 0;
			size_t Verticies	= 0;
			size_t Indices		= 0;
			std::vector<Ref<Texture2D>> FontAtlases;

			void Reset()
			{
				Characters	= 0;
				Verticies	= 0;
				Indices		= 0;
			}
		};

		Ref<const OrthographicCamera> m_Camera;

		Ref<VertexArray>	m_QuadVertexArray		= nullptr;
		Ref<VertexBuffer>	m_QuadVertexBuffer		= nullptr;
		Ref<IndexBuffer>	m_QuadIndexBuffer		= nullptr;
		Ref<Shader>			m_QuadShader			= nullptr;
		QuadVertex*			m_QuadVertexDataBase	= nullptr;
		QuadVertex*			m_QuadVertexDataPtr		= nullptr;
		QuadBatchStats		m_QuadBatchStats;

		Ref<VertexArray>	m_CircleVertexArray		= nullptr;
		Ref<VertexBuffer>	m_CircleVertexBuffer	= nullptr;
		Ref<IndexBuffer>	m_CircleIndexBuffer		= nullptr;
		Ref<Shader>			m_CircleShader			= nullptr;
		CircleVertex*		m_CircleVertexDataBase	= nullptr;
		CircleVertex*		m_CircleVertexDataPtr	= nullptr;
		CircleBatchStats	m_CircleBatchStats;

		Ref<VertexArray>	m_TextVertexArray		= nullptr;
		Ref<VertexBuffer>	m_TextVertexBuffer		= nullptr;
		Ref<IndexBuffer>	m_TextIndexBuffer		= nullptr;
		Ref<Shader>			m_TextShader			= nullptr;
		TextVertex*			m_TextVertexDataBase	= nullptr;
		TextVertex*			m_TextVertexDataPtr		= nullptr;
		TextBatchStats		m_TextBatchStats;

		Ref<Texture2D> m_WhiteTexture = nullptr;
		std::vector<Ref<MFL::Font>> m_FontStack;

		float ZDepth = -1.0f;
	};
}