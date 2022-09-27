#pragma once

#include <string>
#include <inttypes.h>

namespace monk 
{
	struct BoundaryBox
	{
		int x0, y0;
		int x1, y1;
	};

	class Truetype
	{
	public:
		Truetype(const uint8_t* data);
		~Truetype() = default;

	public:
		bool Init();
		uint32_t FindTable(const std::string& tag) const;
		int FindGlyphIndex(int codepoint);
		int GetGlyfOffset(int glyphIndex);
		bool GetGlyphBox(int glyphIndex, BoundaryBox* box);
		BoundaryBox GetGlyphBitmapBoxSubpixel(int glyph, float scale_x, float scale_y);

	private:
		const uint8_t* m_Data;
		int m_NumGlyphs;
		int m_IndexMap;
		int m_IndexToLocFormat;
		struct {
			int Cmap;
			int Loca;
			int Head;
			int Glyf;
			int Hhea;
			int Hmtx;
		} m_Tables;
	};
}