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

	enum {
		Vmove = 1,
		Vline,
		Vcurve,
		Vcubic
	};

	struct GlyphVertex
	{
		int16_t x, y, cx, cy, cx1, cy1;
		uint8_t type, padding;
	};

	struct SdfChar
	{
		uint8_t* Data;
		int Width;
		int Height;
		int XOffset;
		int YOffset;
		float Advance;

		void Free();
	};

	class Truetype
	{
	public:
		Truetype(const uint8_t* data);
		~Truetype() = default;

	public:
		SdfChar GetCodepointSDF(float scale, int codepoint, int padding, uint8_t onedge, float pixelDistScale);
		float ScaleForPixelHeight(float height);

	private:
		bool Init();
		uint32_t FindTable(const std::string& tag) const;
		int FindGlyphIndex(int codepoint);
		int GetGlyfOffset(int glyphIndex);
		bool GetGlyphBox(int glyphIndex, BoundaryBox* box);
		BoundaryBox GetGlyphBitmapBoxSubpixel(int glyph, float scale_x, float scale_y);
		int GetGlyphShape(int glyph, GlyphVertex** pvertices);
		int CloseShape(GlyphVertex* vertices, int numVertices, int wasOff, int startOff,
			int32_t sx, int32_t sy, int32_t scx, int32_t scy, int32_t cx, int32_t cy);
		int GetCodepointHMetrics(int codepoint);

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