#pragma once

#include "math/Math.h"
#include "graphics/Buffer.h"
#include "graphics/Shader.h"

namespace monk
{
	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D();

	public:
		struct Vertex
		{
			math::vec3 Position;
			math::vec4 Color;
			float TextureID;
			math::vec2 TextureCoords;

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

	public:
		void Begin(const math::mat4& projection);
		void End();

		void SetClearColor(const math::vec4& clearColor);
		void Clear();

		void DrawRect(const math::vec2& position, const math::vec2& size, const math::vec4& color);
		void DrawRoundRect(const math::vec2& position, const math::vec2& size, const math::vec4& color, math::vec4 round = math::vec4(1.0f));
		void DrawCircle(const math::vec2& center, float radius, const math::vec4& color, uint32_t segments = 24);

		void FillRect(const math::vec2& position, const math::vec2& size, const math::vec4& color);
		void FillRoundRect(const math::vec2& position, const math::vec2& size, const math::vec4& color, math::vec4 round = math::vec4(1.0f));
		void FillCircle(const math::vec2& center, float radius, const math::vec4& color, uint32_t segments = 24);

		void DrawTexture(const math::vec2& position, const math::vec2& size, uint32_t textureID);

	private:
		void BeginBatch();
		void EndBatch();

		void Flush();

	private:
		unsigned m_VAO = 0;
		math::mat4 m_ProjectionMatrix = math::mat4(0);

		math::vec4 m_ClearColor = math::vec4(1.0f);

		struct BatchSettings
		{
			size_t MaxQuads = 1000;
			size_t MaxVerticies = MaxQuads * 4;
			size_t MaxIndices = MaxQuads * 6;
		} m_BatchSettings;

		struct BatchStats
		{
			size_t Quads = 0;
			size_t Verticies = 0;
			size_t Indices = 0;

			void Reset()
			{
				Quads = 0;
				Verticies = 0;
				Indices = 0;
			}
		} m_BatchStats;

		struct RenderStats
		{
			size_t Quads = 0;
			size_t Verticies = 0;
			size_t Indices = 0;
			size_t Batches = 0;

			RenderStats& operator+=(const BatchStats batch)
			{
				Quads += batch.Quads;
				Verticies += batch.Verticies;
				Indices += batch.Indices;
				Batches++;

				return *this;
			}

			void Reset()
			{
				Verticies = 0;
				Indices = 0;
				Batches = 0;
			}
		} m_RenderStats;

		VertexBuffer* m_VertexBuffer = nullptr;
		Vertex* m_VertexBufferData = nullptr;
		IndexBuffer* m_IndexBuffer = nullptr;
		Shader* m_Shader = nullptr;
	};
}