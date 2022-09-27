#include "Truetype.h"

#include "utils/FileManager.h"

#include "core/Log.h"
#include "core/Assert.h"

#include <math.h>

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

	void SetVertex(GlyphVertex* v, uint8_t type, int32_t x, int32_t y, int32_t cx, int32_t cy)
	{
		v->type = type;
		v->x = (int16_t)x;
		v->y = (int16_t)y;
		v->cx = (int16_t)cx;
		v->cy = (int16_t)cy;
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

	uint8_t* Truetype::GetCodepointSDF(float scale, int codepoint, int padding, uint8_t onedge, float pixelDistScale, int* width, int* height, int* xOff, int* yOff)
	{
		int glyph = FindGlyphIndex(codepoint);

		BoundaryBox box = GetGlyphBitmapBoxSubpixel(glyph, scale, scale);

		if (box.x0 == box.x1 || box.y0 == box.y0)
			return nullptr;

		box.x0 -= padding;
		box.y0 -= padding;
		box.x1 += padding;
		box.y1 += padding;

		int w = (box.x1 - box.x0);
		int h = (box.y1 - box.y0);

		if (width) 
			*width = w;
		if (height) 
			*height = h;
		if (xOff) 
			*xOff = box.x0;
		if (yOff) 
			*yOff = box.y0;

		float scaleX = scale;
		float scaleY = -scale;
	}

	int Truetype::GetGlyphShape(int glyph, GlyphVertex** pvertices)
	{
		int16_t numberOfContours;
		const uint8_t* endPtsOfContours;
		const uint8_t* data = m_Data;
		GlyphVertex* vertices = 0;
		int num_vertices = 0;
		int g = GetGlyfOffset(glyph);

		*pvertices = NULL;

		if (g < 0) return 0;

		numberOfContours = ToLittleEndian16(data + g);

		if (numberOfContours > 0) {
			uint8_t flags = 0, flagcount;
			int32_t ins, i, j = 0, m, n, next_move, was_off = 0, off, start_off = 0;
			int32_t x, y, cx, cy, sx, sy, scx, scy;
			const uint8_t* points;
			endPtsOfContours = (data + g + 10);
			ins = ToLittleEndian16U(data + g + 10 + numberOfContours * 2);
			points = data + g + 10 + numberOfContours * 2 + 2 + ins;

			n = 1 + ToLittleEndian16U(endPtsOfContours + numberOfContours * 2 - 2);

			m = n + 2 * numberOfContours;  // a loose bound on how many vertices we might need
			// vertices = (GlyphVertex*)STBTT_malloc(m * sizeof(vertices[0]), info->userdata);
			vertices = (GlyphVertex*)malloc(m * sizeof(vertices[0]));
			if (vertices == 0)
				return 0;

			next_move = 0;
			flagcount = 0;

			// in first pass, we load uninterpreted data into the allocated array
			// above, shifted to the end of the array so we won't overwrite it when
			// we create our final data starting from the front

			off = m - n; // starting offset for uninterpreted data, regardless of how m ends up being calculated

			// first load flags

			for (i = 0; i < n; ++i) {
				if (flagcount == 0) {
					flags = *points++;
					if (flags & 8)
						flagcount = *points++;
				}
				else
					--flagcount;
				vertices[off + i].type = flags;
			}

			// now load x coordinates
			x = 0;
			for (i = 0; i < n; ++i) {
				flags = vertices[off + i].type;
				if (flags & 2) {
					int16_t dx = *points++;
					x += (flags & 16) ? dx : -dx; // ???
				}
				else {
					if (!(flags & 16)) {
						x = x + (int16_t)(points[0] * 256 + points[1]);
						points += 2;
					}
				}
				vertices[off + i].x = (int16_t)x;
			}

			// now load y coordinates
			y = 0;
			for (i = 0; i < n; ++i) {
				flags = vertices[off + i].type;
				if (flags & 4) {
					int16_t dy = *points++;
					y += (flags & 32) ? dy : -dy; // ???
				}
				else {
					if (!(flags & 32)) {
						y = y + (int16_t)(points[0] * 256 + points[1]);
						points += 2;
					}
				}
				vertices[off + i].y = (int16_t)y;
			}

			// now convert them to our format
			num_vertices = 0;
			sx = sy = cx = cy = scx = scy = 0;
			for (i = 0; i < n; ++i) {
				flags = vertices[off + i].type;
				x = (int16_t)vertices[off + i].x;
				y = (int16_t)vertices[off + i].y;

				if (next_move == i) {
					if (i != 0)
						num_vertices = CloseShape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);

					// now start the new one
					start_off = !(flags & 1);
					if (start_off) {
						// if we start off with an off-curve point, then when we need to find a point on the curve
						// where we can start, and we need to save some state for when we wraparound.
						scx = x;
						scy = y;
						if (!(vertices[off + i + 1].type & 1)) {
							// next point is also a curve point, so interpolate an on-point curve
							sx = (x + (int32_t)vertices[off + i + 1].x) >> 1;
							sy = (y + (int32_t)vertices[off + i + 1].y) >> 1;
						}
						else {
							// otherwise just use the next point as our start point
							sx = (int32_t)vertices[off + i + 1].x;
							sy = (int32_t)vertices[off + i + 1].y;
							++i; // we're using point i+1 as the starting point, so skip it
						}
					}
					else {
						sx = x;
						sy = y;
					}
					SetVertex(&vertices[num_vertices++], Vmove, sx, sy, 0, 0);
					was_off = 0;
					next_move = 1 + ToLittleEndian16U(endPtsOfContours + j * 2);
					++j;
				}
				else {
					if (!(flags & 1)) { // if it's a curve
						if (was_off) // two off-curve control points in a row means interpolate an on-curve midpoint
							SetVertex(&vertices[num_vertices++], Vcurve, (cx + x) >> 1, (cy + y) >> 1, cx, cy);
						cx = x;
						cy = y;
						was_off = 1;
					}
					else {
						if (was_off)
							SetVertex(&vertices[num_vertices++], Vcurve, x, y, cx, cy);
						else
							SetVertex(&vertices[num_vertices++], Vline, x, y, 0, 0);
						was_off = 0;
					}
				}
			}
			num_vertices = CloseShape(vertices, num_vertices, was_off, start_off, sx, sy, scx, scy, cx, cy);
		}
		else if (numberOfContours < 0) {
			// Compound shapes.
			int more = 1;
			const uint8_t* comp = data + g + 10;
			num_vertices = 0;
			vertices = 0;
			while (more) {
				uint16_t flags, gidx;
				int comp_num_verts = 0, i;
				GlyphVertex* comp_verts = 0, * tmp = 0;
				float mtx[6] = { 1,0,0,1,0,0 }, m, n;

				flags = ToLittleEndian16(comp); comp += 2;
				gidx = ToLittleEndian16(comp); comp += 2;

				if (flags & 2) { // XY values
					if (flags & 1) { // shorts
						mtx[4] = ToLittleEndian16(comp); comp += 2;
						mtx[5] = ToLittleEndian16(comp); comp += 2;
					}
					else {
						mtx[4] = *(int8_t*)comp; comp += 1;
						mtx[5] = *(int8_t*)comp; comp += 1;
					}
				}
				else {
					// @TODO handle matching point
					MONK_ASSERT(false);
				}
				if (flags & (1 << 3)) { // WE_HAVE_A_SCALE
					mtx[0] = mtx[3] = ToLittleEndian16(comp) / 16384.0f; comp += 2;
					mtx[1] = mtx[2] = 0;
				}
				else if (flags & (1 << 6)) { // WE_HAVE_AN_X_AND_YSCALE
					mtx[0] = ToLittleEndian16(comp) / 16384.0f; comp += 2;
					mtx[1] = mtx[2] = 0;
					mtx[3] = ToLittleEndian16(comp) / 16384.0f; comp += 2;
				}
				else if (flags & (1 << 7)) { // WE_HAVE_A_TWO_BY_TWO
					mtx[0] = ToLittleEndian16(comp) / 16384.0f; comp += 2;
					mtx[1] = ToLittleEndian16(comp) / 16384.0f; comp += 2;
					mtx[2] = ToLittleEndian16(comp) / 16384.0f; comp += 2;
					mtx[3] = ToLittleEndian16(comp) / 16384.0f; comp += 2;
				}

				// Find transformation scales.
				m = std::sqrtf(mtx[0] * mtx[0] + mtx[1] * mtx[1]);
				n = std::sqrtf(mtx[2] * mtx[2] + mtx[3] * mtx[3]);

				// Get indexed glyph.
				comp_num_verts = GetGlyphShape(gidx, &comp_verts);
				if (comp_num_verts > 0) {
					// Transform vertices.
					for (i = 0; i < comp_num_verts; ++i) {
						GlyphVertex* v = &comp_verts[i];
						uint16_t x, y;
						x = v->x; y = v->y;
						v->x = (uint16_t)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
						v->y = (uint16_t)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
						x = v->cx; y = v->cy;
						v->cx = (uint16_t)(m * (mtx[0] * x + mtx[2] * y + mtx[4]));
						v->cy = (uint16_t)(n * (mtx[1] * x + mtx[3] * y + mtx[5]));
					}
					// Append vertices.
					tmp = (GlyphVertex*)malloc((num_vertices + comp_num_verts) * sizeof(GlyphVertex));
					if (!tmp) {
						if (vertices) free(vertices);
						if (comp_verts) free(comp_verts);
						return 0;
					}
					if (num_vertices > 0 && vertices) memcpy(tmp, vertices, num_vertices * sizeof(GlyphVertex));
					memcpy(tmp + num_vertices, comp_verts, comp_num_verts * sizeof(GlyphVertex));
					if (vertices) free(vertices);
					vertices = tmp;
					free(comp_verts);
					num_vertices += comp_num_verts;
				}
				// More components ?
				more = flags & (1 << 5);
			}
		}
		else {
			// numberOfCounters == 0, do nothing
		}

		*pvertices = vertices;
		return num_vertices;
	}
	
	int Truetype::CloseShape(GlyphVertex* vertices, int numVertices, int wasOff, int startOff, int32_t sx, int32_t sy, int32_t scx, int32_t scy, int32_t cx, int32_t cy)
	{
		if (startOff) {
			if (wasOff)
				SetVertex(&vertices[numVertices++], Vcurve, (cx + scx) >> 1, (cy + scy) >> 1, cx, cy);
			SetVertex(&vertices[numVertices++], Vcurve, sx, sy, scx, scy);
		}
		else {
			if (wasOff)
				SetVertex(&vertices[numVertices++], Vcurve, sx, sy, cx, cy);
			else
				SetVertex(&vertices[numVertices++], Vline, sx, sy, 0, 0);
		}
		return numVertices;
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