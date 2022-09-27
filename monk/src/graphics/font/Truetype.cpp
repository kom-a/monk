#include "Truetype.h"

#include "utils/FileManager.h"

#include "core/Log.h"

enum {
	PLATFORM_ID_MICROSOFT = 3
};

enum {
	MS_EID_UNICODE_BMP = 1,
	MS_EID_UNICODE_FULL = 10
};

namespace monk
{
	int16_t ToLittleEndian16(const uint8_t* p) { return (p[0] << 8) | p[1]; }
	uint16_t ToLittleEndian16U(const uint8_t* p) { return (p[0] << 8) | p[1]; }
	int32_t ToLittleEndian32(const uint8_t* p) { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; }
	uint32_t ToLittleEndian32U(const uint8_t* p) { return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]; }

	bool Tag(const std::string& tag, const uint8_t* p)
	{
		for (int i = 0; i < tag.size(); i++)
		{
			if (tag[i] != p[i])
				return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Truetype implementation
	//////////////////////////////////////////////////////////////////////////

	Truetype::Truetype(const uint8_t* data)
	{
		m_Data = data;
		Init();
	}

	uint32_t Truetype::FindTable(const std::string& tag) const
	{
		int32_t numTables = ToLittleEndian16U(m_Data + 4);

		for (int i = 0; i < numTables; i++)
		{
			uint32_t tableTagOffset = i * 16 + 12;
			if (Tag(tag, m_Data + tableTagOffset))
				return ToLittleEndian32U(m_Data + tableTagOffset + 8);
		}

		return 0;
	}

	int Truetype::FindGlyphIndex(int codepoint)
	{
		uint16_t format = ToLittleEndian16U(m_Data + m_IndexMap+ 0);

		if (format != 4) // 4- standard mapping for windows fonts
		{
			LOG_ERROR("Font format not supported");
			return 0;
		}

		uint16_t segcount = ToLittleEndian16U(m_Data + m_IndexMap + 6) >> 1;
		uint16_t searchRange = ToLittleEndian16U(m_Data + m_IndexMap + 8) >> 1;
		uint16_t entrySelector = ToLittleEndian16U(m_Data + m_IndexMap + 10);
		uint16_t rangeShift = ToLittleEndian16U(m_Data + m_IndexMap + 12) >> 1;

		// do a binary search of the segments
		uint32_t endCount = m_IndexMap + 14;
		uint32_t search = endCount;

		if (codepoint > 0xffff)
			return 0;

		// they lie from endCount .. endCount + segCount
		// but searchRange is the nearest power of two, so...
		if (codepoint >= ToLittleEndian16U(m_Data + search + rangeShift * 2))
			search += rangeShift * 2;

		// now decrement to bias correctly to find smallest
		search -= 2;
		while (entrySelector) {
			uint16_t end;
			searchRange >>= 1;
			end = ToLittleEndian16U(m_Data + search + searchRange * 2);
			if (codepoint > end)
				search += searchRange * 2;
			--entrySelector;
		}
		search += 2;

		{
			uint16_t offset, start, last;
			uint16_t item = (uint16_t)((search - endCount) >> 1);

			start = ToLittleEndian16U(m_Data + m_IndexMap + 14 + segcount * 2 + 2 + 2 * item);
			last = ToLittleEndian16U(m_Data + endCount + 2 * item);
			if (codepoint < start || codepoint > last)
				return 0;

			offset = ToLittleEndian16U(m_Data + m_IndexMap + 14 + segcount * 6 + 2 + 2 * item);
			if (offset == 0)
				return (uint16_t)(codepoint + ToLittleEndian16(m_Data + m_IndexMap + 14 + segcount * 4 + 2 + 2 * item));

			return ToLittleEndian16U(m_Data + offset + (codepoint - start) * 2 + m_IndexMap + 14 + segcount * 6 + 2 + 2 * item);
		}
	}

	int Truetype::GetGlyfOffset(int glyphIndex)
	{
		int g1, g2;

		if (glyphIndex >= m_NumGlyphs) 
			return -1; // glyph index out of range
		if (m_IndexToLocFormat >= 2)    
			return -1; // unknown index->glyph map format

		if (m_IndexToLocFormat == 0) 
		{
			g1 = m_Tables.Glyf + ToLittleEndian16U(m_Data + m_Tables.Loca + glyphIndex * 2) * 2;
			g2 = m_Tables.Glyf + ToLittleEndian16U(m_Data + m_Tables.Loca + glyphIndex * 2 + 2) * 2;
		}
		else 
		{
			g1 = m_Tables.Glyf + ToLittleEndian32U(m_Data + m_Tables.Loca + glyphIndex * 4);
			g2 = m_Tables.Glyf + ToLittleEndian32U(m_Data + m_Tables.Loca + glyphIndex * 4 + 4);
		}

		return g1 == g2 ? -1 : g1; // if length is 0, return -1
	}

	bool Truetype::GetGlyphBox(int glyphIndex, BoundaryBox* box)
	{
		int g = GetGlyfOffset(glyphIndex);
		if (g < 0) 
			return false;

		if (box)
		{
			box->x0 = ToLittleEndian16(m_Data + g + 2);
			box->y0 = ToLittleEndian16(m_Data + g + 4);
			box->x1 = ToLittleEndian16(m_Data + g + 6);
			box->y1 = ToLittleEndian16(m_Data + g + 8);
		}

		return true;
	}

	BoundaryBox Truetype::GetGlyphBitmapBoxSubpixel(int glyph, float scaleX, float scaleY)
	{
		BoundaryBox box = { 0 };
		BoundaryBox result = { 0 };

		if (GetGlyphBox(glyph, &box))
		{
			result.x0 = floor(box.x0 * scaleX);
			result.y0 = floor(-box.y1 * scaleY);
			result.x1 = ceil(box.x1 * scaleX);
			result.y1 = ceil(-box.y0 * scaleY);
		}
		
		return result;
	}

	bool Truetype::Init()
	{
		m_Tables.Cmap = FindTable("cmap");
		m_Tables.Loca = FindTable("loca");
		m_Tables.Head = FindTable("head");
		m_Tables.Glyf = FindTable("glyf");
		m_Tables.Hhea = FindTable("hhea");
		m_Tables.Hmtx = FindTable("hmtx");

		if (!m_Tables.Cmap
			|| !m_Tables.Head
			|| !m_Tables.Hhea
			|| !m_Tables.Hmtx
			|| !m_Tables.Glyf
			|| !m_Tables.Loca)
		{
			LOG_ERROR("Failed to parse ttf");
			return false;
		}

		int maxpTable = FindTable("maxp");
		if (maxpTable)
			m_NumGlyphs = ToLittleEndian16U(m_Data + maxpTable + 4);
		else
			m_NumGlyphs = 0xffff;

		int numTables = ToLittleEndian16U(m_Data + m_Tables.Cmap + 2);
		m_IndexMap = 0;
		for (int i = 0; i < numTables; ++i) {
			uint32_t encodingRecord = m_Tables.Cmap + 4 + 8 * i;
			if (ToLittleEndian16U(m_Data + encodingRecord) == PLATFORM_ID_MICROSOFT)
			{
				switch (ToLittleEndian16U(m_Data + encodingRecord + 2)) {
				case MS_EID_UNICODE_BMP:
				case MS_EID_UNICODE_FULL:
					m_IndexMap = m_Tables.Cmap + ToLittleEndian32U(m_Data + encodingRecord + 4);
					break;
				}
			}
		}
		if (m_IndexMap == 0)
		{
			LOG_ERROR("Failed to find index map");
			return false;
		}
		
		m_IndexToLocFormat = ToLittleEndian16U(m_Data + m_Tables.Head + 50);
		return true;
	}
}