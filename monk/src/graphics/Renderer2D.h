#pragma once

#include "core/Memory.h"
#include "math/Math.h"
#include "graphics/Buffer.h"
#include "graphics/Shader.h"
#include "graphics/Texture2D.h"
#include "graphics/OrthographicCamera.h"

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
		void Begin(const OrthographicCamera& camera);
		void End();

		void DrawQuad(const math::vec2& position, const math::vec2& size, const math::vec4& color);
		void DrawTexture(const math::vec2& position, const math::vec2& size, const Texture2D& texture);

	private:
		void BeginBatch();
		void EndBatch();

		void Flush();

	private:
		unsigned m_VAO = 0;
		
		const OrthographicCamera* m_Camera;

		struct BatchSettings
		{
			const size_t MaxQuads = 1000;
			const size_t MaxVerticies = MaxQuads * 4;
			const size_t MaxIndices = MaxQuads * 6;
			const size_t MaxTextures = 16;
		} m_BatchSettings;

		struct BatchStats
		{
			size_t Quads = 0;
			size_t Verticies = 0;
			size_t Indices = 0;
			std::vector<uint32_t> Textures;
			size_t TextureIndex = 1;

			void Reset()
			{
				Quads = 0;
				Verticies = 0;
				Indices = 0;
				TextureIndex = 1;
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
		Scope<Texture2D> m_WhiteTexture = nullptr;
	};
}