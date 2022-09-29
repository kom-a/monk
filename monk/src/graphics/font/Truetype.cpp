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

	float Cuberoot(float x)
	{
		if (x < 0)
			return -std::powf(-x, 1.0f / 3.0f);
		else
			return  std::powf( x, 1.0f / 3.0f);
	}

	// x^3 + a*x^2 + b*x + c = 0
	int SolveCubic(float a, float b, float c, float* r)
	{
		float s = -a / 3;
		float p = b - a * a / 3;
		float q = a * (2 * a * a - 9 * b) / 27 + c;
		float p3 = p * p * p;
		float d = q * q + 4 * p3 / 27;
		if (d >= 0) {
			float z = std::sqrtf(d);
			float u = (-q + z) / 2;
			float v = (-q - z) / 2;
			u = Cuberoot(u);
			v = Cuberoot(v);
			r[0] = s + u + v;
			return 1;
		}
		else {
			float u = std::sqrtf(-p / 3);
			float v = std::acosf(-std::sqrtf(-27 / p3) * q / 2) / 3;
			float m = std::cosf(v);
			float n = std::cosf(v - 3.141592 / 2) * 1.732050808f;
			r[0] = s + u * 2 * m;
			r[1] = s - u * (m + n);
			r[2] = s - u * (m - n);

			return 3;
		}
	}

	int Equal2f(float* a, float* b)
	{
		return (a[0] == b[0] && a[1] == b[1]);
	}

	int RayIntersectBezier(float orig[2], float ray[2], float q0[2], float q1[2], float q2[2], float hits[2][2])
	{
		float q0perp = q0[1] * ray[0] - q0[0] * ray[1];
		float q1perp = q1[1] * ray[0] - q1[0] * ray[1];
		float q2perp = q2[1] * ray[0] - q2[0] * ray[1];
		float roperp = orig[1] * ray[0] - orig[0] * ray[1];

		float a = q0perp - 2 * q1perp + q2perp;
		float b = q1perp - q0perp;
		float c = q0perp - roperp;

		float s0 = 0., s1 = 0.;
		int num_s = 0;

		if (a != 0.0) {
			float discr = b * b - a * c;
			if (discr > 0.0) {
				float rcpna = -1 / a;
				float d = std::sqrtf(discr);
				s0 = (b + d) * rcpna;
				s1 = (b - d) * rcpna;
				if (s0 >= 0.0 && s0 <= 1.0)
					num_s = 1;
				if (d > 0.0 && s1 >= 0.0 && s1 <= 1.0) {
					if (num_s == 0) s0 = s1;
					++num_s;
				}
			}
		}
		else {
			// 2*b*s + c = 0
			// s = -c / (2*b)
			s0 = c / (-2 * b);
			if (s0 >= 0.0 && s0 <= 1.0)
				num_s = 1;
		}

		if (num_s == 0)
			return 0;
		else {
			float rcp_len2 = 1 / (ray[0] * ray[0] + ray[1] * ray[1]);
			float rayn_x = ray[0] * rcp_len2, rayn_y = ray[1] * rcp_len2;

			float q0d = q0[0] * rayn_x + q0[1] * rayn_y;
			float q1d = q1[0] * rayn_x + q1[1] * rayn_y;
			float q2d = q2[0] * rayn_x + q2[1] * rayn_y;
			float rod = orig[0] * rayn_x + orig[1] * rayn_y;

			float q10d = q1d - q0d;
			float q20d = q2d - q0d;
			float q0rd = q0d - rod;

			hits[0][0] = q0rd + s0 * (2.0f - 2.0f * s0) * q10d + s0 * s0 * q20d;
			hits[0][1] = a * s0 + b;

			if (num_s > 1) {
				hits[1][0] = q0rd + s1 * (2.0f - 2.0f * s1) * q10d + s1 * s1 * q20d;
				hits[1][1] = a * s1 + b;
				return 2;
			}
			else {
				return 1;
			}
		}
	}

	int ComputeCrossingsX(float x, float y, int nverts, GlyphVertex* verts)
	{
		int i;
		float orig[2], ray[2] = { 1, 0 };
		float y_frac;
		int winding = 0;

		// make sure y never passes through a vertex of the shape
		y_frac = std::fmodf(y, 1.0f);
		if (y_frac < 0.01f)
			y += 0.01f;
		else if (y_frac > 0.99f)
			y -= 0.01f;

		orig[0] = x;
		orig[1] = y;

		// test a ray from (-infinity,y) to (x,y)
		for (i = 0; i < nverts; ++i) {
			if (verts[i].type == Vline) {
				int x0 = (int)verts[i - 1].x, y0 = (int)verts[i - 1].y;
				int x1 = (int)verts[i].x, y1 = (int)verts[i].y;
				if (y > std::min(y0, y1) && y < std::max(y0, y1) && x > std::min(x0, x1)) {
					float x_inter = (y - y0) / (y1 - y0) * (x1 - x0) + x0;
					if (x_inter < x)
						winding += (y0 < y1) ? 1 : -1;
				}
			}
			if (verts[i].type == Vcurve) {
				int x0 = (int)verts[i - 1].x, y0 = (int)verts[i - 1].y;
				int x1 = (int)verts[i].cx, y1 = (int)verts[i].cy;
				int x2 = (int)verts[i].x, y2 = (int)verts[i].y;
				int ax = std::min(x0, std::min(x1, x2)), ay = std::min(y0, std::min(y1, y2));
				int by = std::max(y0, std::max(y1, y2));
				if (y > ay && y < by && x > ax) {
					float q0[2], q1[2], q2[2];
					float hits[2][2];
					q0[0] = (float)x0;
					q0[1] = (float)y0;
					q1[0] = (float)x1;
					q1[1] = (float)y1;
					q2[0] = (float)x2;
					q2[1] = (float)y2;
					if (Equal2f(q0, q1) || Equal2f(q1, q2)) {
						x0 = (int)verts[i - 1].x;
						y0 = (int)verts[i - 1].y;
						x1 = (int)verts[i].x;
						y1 = (int)verts[i].y;
						if (y > std::min(y0, y1) && y < std::max(y0, y1) && x > std::min(x0, x1)) {
							float x_inter = (y - y0) / (y1 - y0) * (x1 - x0) + x0;
							if (x_inter < x)
								winding += (y0 < y1) ? 1 : -1;
						}
					}
					else {
						int num_hits = RayIntersectBezier(orig, ray, q0, q1, q2, hits);
						if (num_hits >= 1)
							if (hits[0][0] < 0)
								winding += (hits[0][1] < 0 ? -1 : 1);
						if (num_hits >= 2)
							if (hits[1][0] < 0)
								winding += (hits[1][1] < 0 ? -1 : 1);
					}
				}
			}
		}
		return winding;
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

	FontChar Truetype::GetCodepointSDF(float scale, int codepoint, int padding, uint8_t onedge, float pixelDistScale)
	{
		int glyph = FindGlyphIndex(codepoint);
		FontChar result = { nullptr };

		if (scale == 0)
			return { nullptr };

		BoundaryBox box = GetGlyphBitmapBoxSubpixel(glyph, scale, scale);

		if (box.x0 == box.x1 || box.y0 == box.y1)
			return { nullptr };

		box.x0 -= padding;
		box.y0 -= padding;
		box.x1 += padding;
		box.y1 += padding;

		int w = (box.x1 - box.x0);
		int h = (box.y1 - box.y0);

		result.Width = w;
		result.Height = h;
		result.XOffset = box.x0;
		result.YOffset = box.y0;

		float scaleX = scale;
		float scaleY = -scale;

		{
			int x, y, i, j;
			float* precompute;
			GlyphVertex* verts;
			int num_verts = GetGlyphShape(glyph, &verts);
			uint8_t* data = (unsigned char*)malloc(w * h);
			precompute = (float*)malloc(num_verts * sizeof(float));

			for (i = 0, j = num_verts - 1; i < num_verts; j = i++) {
				if (verts[i].type == Vline) {
					float x0 = verts[i].x * scaleX, y0 = verts[i].y * scaleY;
					float x1 = verts[j].x * scaleX, y1 = verts[j].y * scaleY;
					float dist = std::sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
					precompute[i] = (dist == 0) ? 0.0f : 1.0f / dist;
				}
				else if (verts[i].type == Vcurve) {
					float x2 = verts[j].x * scaleX, y2 = verts[j].y * scaleY;
					float x1 = verts[i].cx * scaleX, y1 = verts[i].cy * scaleY;
					float x0 = verts[i].x * scaleX, y0 = verts[i].y * scaleY;
					float bx = x0 - 2 * x1 + x2, by = y0 - 2 * y1 + y2;
					float len2 = bx * bx + by * by;
					if (len2 != 0.0f)
						precompute[i] = 1.0f / (bx * bx + by * by);
					else
						precompute[i] = 0.0f;
				}
				else
					precompute[i] = 0.0f;
			}

			for (y = box.y0; y < box.y1; ++y) {
				for (x = box.x0; x < box.x1; ++x) {
					float val;
					float min_dist = 999999.0f;
					float sx = (float)x + 0.5f;
					float sy = (float)y + 0.5f;
					float x_gspace = (sx / scaleX);
					float y_gspace = (sy / scaleY);

					int winding = ComputeCrossingsX(x_gspace, y_gspace, num_verts, verts); // @OPTIMIZE: this could just be a rasterization, but needs to be line vs. non-tesselated curves so a new path

					for (i = 0; i < num_verts; ++i) {
						float x0 = verts[i].x * scaleX, y0 = verts[i].y * scaleY;

						if (verts[i].type == Vline && precompute[i] != 0.0f) {
							float x1 = verts[i - 1].x * scaleX, y1 = verts[i - 1].y * scaleY;

							float dist, dist2 = (x0 - sx) * (x0 - sx) + (y0 - sy) * (y0 - sy);
							if (dist2 < min_dist * min_dist)
								min_dist = std::sqrtf(dist2);

							// coarse culling against bbox
							//if (sx > STBTT_min(x0,x1)-min_dist && sx < STBTT_max(x0,x1)+min_dist &&
							//    sy > STBTT_min(y0,y1)-min_dist && sy < STBTT_max(y0,y1)+min_dist)
							dist = std::fabsf((x1 - x0) * (y0 - sy) - (y1 - y0) * (x0 - sx)) * precompute[i];
							MONK_ASSERT(i != 0);
							if (dist < min_dist) {
								// check position along line
								// x' = x0 + t*(x1-x0), y' = y0 + t*(y1-y0)
								// minimize (x'-sx)*(x'-sx)+(y'-sy)*(y'-sy)
								float dx = x1 - x0, dy = y1 - y0;
								float px = x0 - sx, py = y0 - sy;
								// minimize (px+t*dx)^2 + (py+t*dy)^2 = px*px + 2*px*dx*t + t^2*dx*dx + py*py + 2*py*dy*t + t^2*dy*dy
								// derivative: 2*px*dx + 2*py*dy + (2*dx*dx+2*dy*dy)*t, set to 0 and solve
								float t = -(px * dx + py * dy) / (dx * dx + dy * dy);
								if (t >= 0.0f && t <= 1.0f)
									min_dist = dist;
							}
						}
						else if (verts[i].type == Vcurve) {
							float x2 = verts[i - 1].x * scaleX, y2 = verts[i - 1].y * scaleY;
							float x1 = verts[i].cx * scaleX, y1 = verts[i].cy * scaleY;
							float box_x0 = std::fminf(std::fminf(x0, x1), x2);
							float box_y0 = std::fminf(std::fminf(y0, y1), y2);
							float box_x1 = std::fmaxf(std::fmaxf(x0, x1), x2);
							float box_y1 = std::fmaxf(std::fmaxf(y0, y1), y2);
							// coarse culling against bbox to avoid computing cubic unnecessarily
							if (sx > box_x0 - min_dist && sx < box_x1 + min_dist && sy > box_y0 - min_dist && sy < box_y1 + min_dist) {
								int num = 0;
								float ax = x1 - x0, ay = y1 - y0;
								float bx = x0 - 2 * x1 + x2, by = y0 - 2 * y1 + y2;
								float mx = x0 - sx, my = y0 - sy;
								float res[3] = { 0.f,0.f,0.f };
								float px, py, t, it, dist2;
								float a_inv = precompute[i];
								if (a_inv == 0.0) { // if a_inv is 0, it's 2nd degree so use quadratic formula
									float a = 3 * (ax * bx + ay * by);
									float b = 2 * (ax * ax + ay * ay) + (mx * bx + my * by);
									float c = mx * ax + my * ay;
									if (a == 0.0) { // if a is 0, it's linear
										if (b != 0.0) {
											res[num++] = -c / b;
										}
									}
									else {
										float discriminant = b * b - 4 * a * c;
										if (discriminant < 0)
											num = 0;
										else {
											float root = std::sqrtf(discriminant);
											res[0] = (-b - root) / (2 * a);
											res[1] = (-b + root) / (2 * a);
											num = 2; // don't bother distinguishing 1-solution case, as code below will still work
										}
									}
								}
								else {
									float b = 3 * (ax * bx + ay * by) * a_inv; // could precompute this as it doesn't depend on sample point
									float c = (2 * (ax * ax + ay * ay) + (mx * bx + my * by)) * a_inv;
									float d = (mx * ax + my * ay) * a_inv;
									num = SolveCubic(b, c, d, res);
								}
								dist2 = (x0 - sx) * (x0 - sx) + (y0 - sy) * (y0 - sy);
								if (dist2 < min_dist * min_dist)
									min_dist = std::sqrtf(dist2);

								if (num >= 1 && res[0] >= 0.0f && res[0] <= 1.0f) {
									t = res[0], it = 1.0f - t;
									px = it * it * x0 + 2 * t * it * x1 + t * t * x2;
									py = it * it * y0 + 2 * t * it * y1 + t * t * y2;
									dist2 = (px - sx) * (px - sx) + (py - sy) * (py - sy);
									if (dist2 < min_dist * min_dist)
										min_dist = std::sqrtf(dist2);
								}
								if (num >= 2 && res[1] >= 0.0f && res[1] <= 1.0f) {
									t = res[1], it = 1.0f - t;
									px = it * it * x0 + 2 * t * it * x1 + t * t * x2;
									py = it * it * y0 + 2 * t * it * y1 + t * t * y2;
									dist2 = (px - sx) * (px - sx) + (py - sy) * (py - sy);
									if (dist2 < min_dist * min_dist)
										min_dist = std::sqrtf(dist2);
								}
								if (num >= 3 && res[2] >= 0.0f && res[2] <= 1.0f) {
									t = res[2], it = 1.0f - t;
									px = it * it * x0 + 2 * t * it * x1 + t * t * x2;
									py = it * it * y0 + 2 * t * it * y1 + t * t * y2;
									dist2 = (px - sx) * (px - sx) + (py - sy) * (py - sy);
									if (dist2 < min_dist * min_dist)
										min_dist = std::sqrtf(dist2);
								}
							}
						}
					}
					if (winding == 0)
						min_dist = -min_dist;  // if outside the shape, value is negative
					val = onedge + pixelDistScale * min_dist;
					if (val < 0)
						val = 0;
					else if (val > 255)
						val = 255;
					data[(y - box.y0) * w + (x - box.x0)] = (unsigned char)val;
				}
			}
			free(precompute);
			free(verts);
			result.Data = data;
		}
		result.Advance = GetCodepointHMetrics(codepoint);
		return result;
	}

	float Truetype::ScaleForPixelHeight(float height)
	{
		int fheight = ToLittleEndian16(m_Data + m_Tables.Hhea + 4) - ToLittleEndian16(m_Data + m_Tables.Hhea + 6);
		return (float)height / fheight;
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

	int Truetype::GetCodepointHMetrics(int codepoint)
	{
		int glyph = FindGlyphIndex(codepoint);
		int advanceWidth = 0;

		uint16_t numOfLongHorMetrics = ToLittleEndian16U(m_Data + m_Tables.Hhea + 34);
		if (glyph < numOfLongHorMetrics)
			advanceWidth = ToLittleEndian16(m_Data + m_Tables.Hmtx + 4 * glyph);
		else
			advanceWidth = ToLittleEndian16(m_Data + m_Tables.Hmtx + 4 * (numOfLongHorMetrics - 1));

		return advanceWidth;
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

	void FontChar::Free()
	{
		delete[] Data;
	}
}