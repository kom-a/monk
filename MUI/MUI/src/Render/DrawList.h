#pragma once

#include <MFL/MFL.h>

#include "VertexBuffer.h"

namespace mui
{
	struct DrawListBufferStats
	{
		uint32_t Quads			= 0;
		uint32_t VerticesCount	= 0;
		uint32_t IndicesCount	= 0;

		void Reset()
		{
			Quads				= 0;
			VerticesCount		= 0;
			IndicesCount		= 0;
		}
	};

	struct DrawListBuffer
	{
		DrawListBuffer() { Clear(); }

		std::vector<Vertex> Buffer;
		DrawListBufferStats BufferStats;

		void Clear()
		{
			Buffer.clear();
			BufferStats.Reset();
		}
	};

	class DrawList
	{
	public:
		DrawList();

		void DrawQuad(Vec2f position, Vec2f size, Vec4f color);
		void DrawCircle(Vec2f center, float diameter, Vec4f color);
		void DrawString(std::string_view text, Vec2f position, float fontSize, Vec4f color, const MFL::Font& font);
		
		void Clear();

		void SetYOffset(float yOffset);

		static uint32_t MaxQuadCount();

	private:
		DrawListBuffer* GetCurrentBuffer();
		DrawListBuffer* NextBatch();

	private:
		friend class Renderer;

		std::vector<DrawListBuffer> m_DrawList;
		size_t m_CurrentDrawListIndex;

		float m_YOffset = 0.0f;
	};
}