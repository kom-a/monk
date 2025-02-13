#include "DrawList.h"

namespace mui
{
	DrawList::DrawList()
	{
		m_DrawList.emplace_back();
		m_CurrentDrawListIndex = 0;
		Clear();
	}

	void DrawList::DrawQuad(Vec2f position, Vec2f size, Vec4f color)
	{
		DrawListBuffer* buffer = GetCurrentBuffer();
		position.Y += m_YOffset;

		if (buffer->BufferStats.Quads >= MaxQuadCount())
			buffer = NextBatch();

		buffer->Buffer.emplace_back(Vec2f(position.X			, position.Y			), color, Vec2f(-1.0f, -1.0f));
		buffer->Buffer.emplace_back(Vec2f(position.X + size.X	, position.Y			), color, Vec2f(-1.0f, -1.0f));
		buffer->Buffer.emplace_back(Vec2f(position.X + size.X	, position.Y + size.Y	), color, Vec2f(-1.0f, -1.0f));
		buffer->Buffer.emplace_back(Vec2f(position.X			, position.Y + size.Y	), color, Vec2f(-1.0f, -1.0f));

		buffer->BufferStats.Quads			+= 1;
		buffer->BufferStats.VerticesCount	+= 4;
		buffer->BufferStats.IndicesCount	+= 6;
	}

	void DrawList::DrawCircle(Vec2f center, float diameter, Vec4f color)
	{
		DrawListBuffer* buffer = GetCurrentBuffer();
		center.Y += m_YOffset;

		if (buffer->BufferStats.Quads >= MaxQuadCount())
			buffer = NextBatch();

		float radius = diameter / 2;

		Vec2f position = Vec2f(center.X - radius, center.Y - radius);
		Vec2f size = Vec2f(diameter, diameter);

		buffer->Buffer.emplace_back(Vec2f(position.X			, position.Y			), color, center, radius);
		buffer->Buffer.emplace_back(Vec2f(position.X + size.X	, position.Y			), color, center, radius);
		buffer->Buffer.emplace_back(Vec2f(position.X + size.X	, position.Y + size.Y	), color, center, radius);
		buffer->Buffer.emplace_back(Vec2f(position.X			, position.Y + size.Y	), color, center, radius);

		buffer->BufferStats.Quads += 1;
		buffer->BufferStats.VerticesCount += 4;
		buffer->BufferStats.IndicesCount += 6;
	}

	void DrawList::DrawString(std::string_view text, Vec2f position, float fontSize, Vec4f color, const MFL::Font& font)
	{
		position.Y += m_YOffset;

		Vec2f startPosition = position;

		for (uint32_t c : text)
		{
			DrawListBuffer* buffer = GetCurrentBuffer();

			if (buffer->BufferStats.Quads >= MaxQuadCount())
				buffer = NextBatch();

			const MFL::GlyphData& glyphData = font.GetGlyphDataByUnicode(c);

			float scale = font.GetScaleForFontSize(fontSize);
			float width = glyphData.Width * scale;
			float height = glyphData.Height * scale;
			float ascender = glyphData.Ascender * scale;
			float descender = glyphData.Descender * scale;
			float advance = glyphData.Advance == 0 ? width : glyphData.Advance * scale;

			if (c == ' ')
			{
				position.X += advance;
				continue;
			}
			else if (c == '\n')
			{
				MFL::VerticalMetrics hm = font.GetVerticalMetrics();
				position.X = startPosition.X;
				position.Y += (hm.LineHeight + hm.LineGap) * scale;
				continue;
			}

			buffer->Buffer.emplace_back(
				Vec2f(position.X, position.Y - ascender), 
				color, 
				Vec2f(glyphData.UV_TopLeft.U, glyphData.UV_TopLeft.V));

			buffer->Buffer.emplace_back(
				Vec2f(position.X + width, position.Y - ascender), 
				color, 
				Vec2f(glyphData.UV_TopRight.U, glyphData.UV_TopRight.V));

			buffer->Buffer.emplace_back(
				Vec2f(position.X + width, position.Y - ascender + height), 
				color, 
				Vec2f(glyphData.UV_BottomRight.U, glyphData.UV_BottomRight.V));

			buffer->Buffer.emplace_back(
				Vec2f(position.X, position.Y - ascender + height),
				color, 
				Vec2f(glyphData.UV_BottomLeft.U, glyphData.UV_BottomLeft.V));

			position.X += advance;

			buffer->BufferStats.Quads += 1;
			buffer->BufferStats.VerticesCount += 4;
			buffer->BufferStats.IndicesCount += 6;
		}
	}

	DrawListBuffer* DrawList::GetCurrentBuffer()
	{
		return &m_DrawList[m_CurrentDrawListIndex];
	}

	void DrawList::Clear()
	{
		for (size_t i = 0; i < m_DrawList.size(); i++)
			m_DrawList[i].Clear();

		m_CurrentDrawListIndex = 0;
	}

	void DrawList::SetYOffset(float yOffset)
	{
		m_YOffset = yOffset;
	}

	uint32_t DrawList::MaxQuadCount()
	{
		return 2048;
	}

	DrawListBuffer* DrawList::NextBatch()
	{
		m_CurrentDrawListIndex++;

		if (m_CurrentDrawListIndex >= m_DrawList.size())
		{
			m_DrawList.emplace_back();
			return &m_DrawList.back();
		}

		return &m_DrawList.at(m_CurrentDrawListIndex);
	}

}


