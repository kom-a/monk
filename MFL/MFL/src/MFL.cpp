#include "MFL.h"

#include <fstream>
#include <iostream>
#include <optional>

#include "Log.h"

namespace MFL
{
	TTF::TTF(const std::filesystem::path& filename)
	{
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<char> buffer(size);
		if (file.read(buffer.data(), size))
		{
			Scriptorium::Reader reader(buffer.data(), Scriptorium::Endianness::BIG);
			Parse(reader);
		}
	}

	void TTF::Parse(Scriptorium::Reader& reader)
	{
		offsetTable.Parse(reader);

		for (size_t i = 0; i < offsetTable.num_tables; i++)
		{
			DirTableEntry dirTableEntry(reader);

			m_TableLocation.try_emplace(dirTableEntry.tag, dirTableEntry.offset);

			if (dirTableEntry.tag == "head")
			{
				LOG_INFO("=============================");
				LOG_INFO("Head table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				head.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "cvt ")
			{
				LOG_INFO("=============================");
				LOG_INFO("cvt table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				cvt.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "prep")
			{
				LOG_INFO("=============================");
				LOG_INFO("prep table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				prep.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "kern")
			{
				LOG_INFO("=============================");
				LOG_INFO("kern table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				kern.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "hhea")
			{
				LOG_INFO("=============================");
				LOG_INFO("hhea table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				hhea.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "post")
			{
				LOG_INFO("=============================");
				LOG_INFO("Post table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				post.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "OS/2")
			{
				LOG_INFO("=============================");
				LOG_INFO("OS/2 table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				os2.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "name")
			{
				LOG_INFO("=============================");
				LOG_INFO("Name table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				name.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "maxp")
			{
				LOG_INFO("=============================");
				LOG_INFO("Maxp table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				maxp.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "fpgm")
			{
				LOG_INFO("=============================");
				LOG_INFO("Fpgm table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				fpgm.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "cmap")
			{
				LOG_INFO("=============================");
				LOG_INFO("Cmap table");
				LOG_INFO("\t\toffset: {0}", dirTableEntry.offset);
				LOG_INFO("\t\tlength: {0}", dirTableEntry.length);
				LOG_INFO("\t\tchecksum: {0}", dirTableEntry.checksum);

				reader.PushCursor(dirTableEntry.offset);
				cmap.Parse(reader);
				reader.PopCursor();
			}
		}

		size_t location_table_start		= m_TableLocation["loca"];
		size_t glyf_table_start			= m_TableLocation["glyf"];

		reader.PushCursor();

		for (size_t i = 0; i < maxp.num_glyphs; i++)
		{
			reader.SetCursor(location_table_start + i * (head.index_to_loc_format ? 4 : 2));
			size_t glyf_offset = head.index_to_loc_format ? reader.ReadUInt32() : reader.ReadUInt16() * 2;

			Glyf glyf;
			reader.SetCursor(glyf_table_start + glyf_offset);
			glyf.Parse(reader);

			glyfs.push_back(glyf);
		}

		// Compound glyphs
		for (Glyf& glyf : glyfs)
		{
			if (glyf.number_of_contours != -1)
				continue;

			glyf.number_of_contours = 0;

			for (CompoundGlyph& compound : glyf.compounds)
			{
				const Glyf& component = glyfs[compound.glyph_index];

				// TODO: Handle multiple nested compound glyphs
				if(component.number_of_contours == -1)
					continue;

				for (auto end_point : component.value.end_pts_of_contours)
					glyf.value.end_pts_of_contours.push_back(end_point + glyf.value.coordinates.size());

				for (const auto& point : component.value.coordinates)
				{
					GlyfPoint new_point;
					new_point.x = point.x * compound.x_scale + compound.x_offset;
					new_point.y = point.y * compound.y_scale + compound.y_offset;

					glyf.value.coordinates.push_back(new_point);
				}

				glyf.value.flags.insert(glyf.value.flags.begin(), component.value.flags.begin(), component.value.flags.end());
				
				glyf.number_of_contours += component.number_of_contours;
			}
		}

		reader.PopCursor();
	}

	Font::Font(const std::filesystem::path& filename)
		: 
		m_Ttf(filename), 
		m_Atlas(m_Ttf)
	{
	}

	const MFL::TTF& Font::GetTTF() const
	{
		return m_Ttf;
	}

	const MFL::Atlas& Font::GetAtlas() const
	{
		return m_Atlas;
	}

	static std::vector<std::vector<GlyfPoint>> SplitByContour(const Glyf& glyf)
	{
		std::vector<std::vector<GlyfPoint>> result;

		for (size_t i = 0; i < glyf.value.coordinates.size(); i++)
		{
			if (i == 0 || std::find(
				glyf.value.end_pts_of_contours.begin(),
				glyf.value.end_pts_of_contours.end(),
				i - 1) != glyf.value.end_pts_of_contours.end())
			{
				result.emplace_back();
			}

			result.back().push_back(glyf.value.coordinates[i]);
		}

		return result;
	}

	static std::vector<std::vector<GlyfPoint>> GetGlyphPoints(const Glyf& glyf)
	{
		std::vector<std::vector<GlyfPoint>> points;
		std::vector<std::vector<GlyfPoint>> contours = SplitByContour(glyf);

		for (const auto& contour : contours)
		{
			points.emplace_back();

			for (size_t i = 0; i < contour.size(); i++)
			{
				const GlyfPoint& p = contour[i];

				if (points.back().size())
				{
					if (p.on_curve && points.back().back().on_curve)
					{
						GlyfPoint dummyPoint;
						dummyPoint.x = (p.x + points.back().back().x) / 2;
						dummyPoint.y = (p.y + points.back().back().y) / 2;
						dummyPoint.on_curve = false;
						points.back().push_back(dummyPoint);
					}
					else if (!p.on_curve && !points.back().back().on_curve)
					{
						GlyfPoint dummyPoint;
						dummyPoint.x = (p.x + points.back().back().x) / 2;
						dummyPoint.y = (p.y + points.back().back().y) / 2;
						dummyPoint.on_curve = true;
						points.back().push_back(dummyPoint);
					}
				}

				points.back().push_back(p);

				if (i == contour.size() - 1 && !points.back().back().on_curve)
					points.back().push_back(points.back().front());
			}

			GlyfPoint dummyPoint;
			dummyPoint.x = (points.back().front().x + points.back().back().x) / 2;
			dummyPoint.y = (points.back().front().y + points.back().back().y) / 2;
			dummyPoint.on_curve = false;
			points.back().push_back(dummyPoint);
			points.back().push_back(points.back().front());
		}

		return points;
	}

	MFL::Path GetGlyphPath(const Glyf& glyf)
	{
		MFL::Path result;
		std::vector<std::vector<GlyfPoint>> points = GetGlyphPoints(glyf);

		for (const auto& pts : points)
		{
			result.emplace_back();

			for (size_t i = 0; i < pts.size(); i += 2)
			{
				if (i + 2 < pts.size())
				{
					BezierCurve bc;
					bc.P0 = pts[i + 0];
					bc.P1 = pts[i + 1];
					bc.P2 = pts[i + 2];

					result.back().push_back(bc);
				}
			}
		}

		return result;
	}

	static void fill(std::vector<uint8_t>& buffer, int32_t width, int32_t height, int x, int y)
	{
		int color = 0xff;
		int size = 16;

		for (int i = 0; i < size; i++)
		{
			if (x + i < width)
				buffer[x + i + y * width] = color;
			if (x - i > 0)
				buffer[x - i + y * width] = color;
			if (y + i < height)
				buffer[x + (y + i) * width] = color;
			if (y - i > 0)
				buffer[x + (y - i) * width] = color;
		}
	}

	static std::pair<float, float> CalculateQuadraticRoots(float a, float b, float c)
	{
		std::pair<float, float> result = std::make_pair(FLT_MAX, FLT_MAX);

		const float eps = 1e-5f;

		if (std::abs(a) < eps)
		{
			if (b != 0)
				result.first = -c / b;
		}
		else
		{
			float desc = b * b - 4 * a * c;

			if (desc >= -eps)
			{
				float s = std::sqrt(std::max(0.0f, desc));
				result.first = (-b - s) / (2 * a);
				result.second = (-b + s) / (2 * a);
			}
		}

		return result;
	}

#if 0
	static float Lerp(float start, float end, float t)
	{
		return start + (end - start) * t;
	}

	static GlyfPoint BezierInterpolation(GlyfPoint p0, GlyfPoint p1, GlyfPoint p2, float t)
	{
		float intAX = Lerp(p0.x, p1.x, t);
		float intAY = Lerp(p0.y, p1.y, t);
		
		float intBX = Lerp(p1.x, p2.x, t);
		float intBY = Lerp(p1.y, p2.y, t);

		GlyfPoint res;
		res.x = Lerp(intAX, intBX, t);
		res.y = Lerp(intAY, intBY, t);

		return res;
	}

	static bool IsValidIntersection(const BezierCurve& curve, int x, float t)
	{
		bool isOnCurve = t >= 0 && t < 1;
		bool isToRightOfRay = BezierInterpolation(curve.P0, curve.CP, curve.P1, t).x > x;
		return isOnCurve && isToRightOfRay;
	}


	static int RayCurveIntersections(int x, int y, MFL::BezierCurve curve, bool& inside, float& closestDst)
	{
		const GlyfPoint& p0 = curve.P0;
		const GlyfPoint& p1 = curve.P1;
		const GlyfPoint& p2 = curve.P2;

		//if (p0.y < y && p1.y <= y && p2.y < y)
		//	return 0;
		//if (p0.y > y && p1.y >= y && p2.y >= y)
		//	return 0;

		float ax = p0.x - 2 * p1.x + p2.x;
		float bx = 2 * (p1.x - p0.x);
		float cx = p0.x;

		float ay = p0.y - 2 * p1.y + p2.y;
		float by = 2 * (p1.y - p0.y);
		float cy = p0.y;

		auto [t0, t1] = CalculateQuadraticRoots(ay, by, cy - y);

		float intersection0 = ax * t0 * t0 + bx * t0 + cx;
		float intersection1 = ax * t1 * t1 + bx * t1 + cx;

		const float eps = 0.0f;
		bool valid0 = (t0 >= -eps && t0 <= 1 + eps) && intersection0 > x;
		bool valid1 = (t1 >= -eps && t1 <= 1 + eps) && intersection1 > x;

		if (valid0 || valid1)
		{
			bool use0 = (valid0 && valid1) ? intersection0 < intersection1 : valid0;
			float intersectDst = use0 ? intersection0 : intersection1;

			bool isCloser = intersectDst < closestDst;

			if (isCloser)
			{
				float intersectT = use0 ? t0 : t1;
				float gradY = 2 * ay * intersectT * by;
				inside = gradY < 0;
				closestDst = intersectDst;
			}
		}

		int qwe = 0;
		if (valid0) qwe++;
		if (valid1) qwe++;
		return qwe;
	}
#endif // 0

	static float Lerp(float start, float end, float t)
	{
		return start + (end - start) * t;
	}

	static GlyfPoint BezierInterpolation(GlyfPoint p0, GlyfPoint p1, GlyfPoint p2, float t)
	{
		float intAX = Lerp(p0.x, p1.x, t);
		float intAY = Lerp(p0.y, p1.y, t);

		float intBX = Lerp(p1.x, p2.x, t);
		float intBY = Lerp(p1.y, p2.y, t);

		GlyfPoint res;
		res.x = Lerp(intAX, intBX, t);
		res.y = Lerp(intAY, intBY, t);

		return res;
	}

	struct Parabola
	{
		float AX;
		float AY;

		float BX;
		float BY;

		float CX;
		float CY;
	};

	static Parabola CalculateCoefficients(const BezierCurve& curve)
	{
		Parabola result;

		result.AX = curve.P0.x - 2 * curve.P1.x + curve.P2.x;
		result.BX = 2 * (curve.P1.x - curve.P0.x);
		result.CX = curve.P0.x;

		result.AY = curve.P0.y - 2 * curve.P1.y + curve.P2.y;
		result.BY = 2 * (curve.P1.y - curve.P0.y);
		result.CY = curve.P0.y;

		return result;
	}

	std::optional<Intersection> RayCurveIntersection(int x, int y, const BezierCurve& curve)
	{
		std::optional<Intersection> intersection = std::nullopt;

		const GlyfPoint& p0 = curve.P0;
		const GlyfPoint& p1 = curve.P1;
		const GlyfPoint& p2 = curve.P2;

		//if (p0.y < y && p1.y < y && p2.y < y)
		//	return std::nullopt;
		//if (p0.y > y && p1.y > y && p2.y > y)
		//	return std::nullopt;

		Parabola p = CalculateCoefficients(curve);

		auto [t0, t1] = CalculateQuadraticRoots(p.AY, p.BY, p.CY - y);
		
		float off = 0;
		while (t0 == t1 && t0 != FLT_MAX)
		{
			off += p.AY >= 0 ? 1.0f : -1.0f;

			auto [tt0, tt1] = CalculateQuadraticRoots(p.AY, p.BY, p.CY - y + off);

			t0 = tt0;
			t1 = tt1;
		}
		
		float intersect0 = p.AX * t0 * t0 + p.BX * t0 + p.CX;
		float intersect1 = p.AX * t1 * t1 + p.BX * t1 + p.CX;

		const float eps = 0;
		bool valid0 = (t0 >= -eps && t0 <= 1 + eps) && intersect0 > x;
		bool valid1 = (t1 >= -eps && t1 <= 1 + eps) && intersect1 > x;

		if (valid0 && valid1)
		{
			intersection = Intersection();
			intersection->T = (intersect0 < intersect1) ? t0 : t1;
			intersection->Distance = ((intersect0 < intersect1) ? intersect0 : intersect1);
		}
		else if (valid0 && !valid1)
		{
			intersection = Intersection();
			intersection->T = t0;
			intersection->Distance = intersect0;
		}
		else if (!valid0 && valid1)
		{
			intersection = Intersection();
			intersection->T = t1;
			intersection->Distance = intersect1;
		}
		else if(!valid0 && !valid1)
		{
			intersection = std::nullopt;
		}
		
		if (intersection)
		{
			float gradY = 2 * p.AY * intersection->T + p.BY;
			intersection->Dir = gradY < 0 ? Direction::Clockwise : Direction::Counterclockwise;

			intersection->x = x;
			intersection->y = y;

			intersection->P0 = p0;
			intersection->P1 = p1;
			intersection->P2 = p2;
		}

		return intersection;
	}

	std::optional<Intersection> RayContourIntersection(int x, int y, const Contour& contour)
	{
		std::optional<Intersection> closest = std::nullopt;

		for (size_t i = 0; i < contour.size(); i++)
		{
			const auto& curve = contour[i];

			std::optional<Intersection> intersection = RayCurveIntersection(x, y, curve);

			const float eps = 1e-5f;

			//if (intersection && std::abs(intersection->T - 1.0f) < eps)
			//{
			//	Parabola p = CalculateCoefficients(curve);

			//	int offset = p.AY >= 0 ? -1 : 1;

			//	intersection = RayCurveIntersection(x, y + offset, curve);
			//}

			if (!closest && intersection)
				closest = intersection;
			else if (intersection && closest && intersection->Dir != closest->Dir && std::abs(intersection->Distance - closest->Distance) < 1e-5f)
			{
				float intersectionT = intersection->T > 0.5f ? 1.0f - eps : eps;
				float closestT = closest->T > 0.5f ? 1.0f - eps : eps;

				GlyfPoint intersectionDelta = BezierInterpolation(curve.P0, curve.P1, curve.P2, intersectionT);
				GlyfPoint closestDelta = BezierInterpolation(closest->P0, closest->P1, closest->P2, closestT);

				if (intersectionDelta.x < closestDelta.x)
					closest = intersection;
			}
			else if (intersection && intersection->Distance < closest->Distance)
			{
				closest = intersection;
			}
		}

		return closest;
	}

	bool IsPointInsidePath(int x, int y, const Path& path)
	{
		std::optional<Intersection> closest = std::nullopt;

		for (const auto& contour : path)
		{
			std::optional<Intersection> intersection = RayContourIntersection(x, y, contour);

			if (!closest && intersection)
				closest = intersection;

			if (intersection && intersection->Distance < closest->Distance)
				closest = intersection;
		}

		if (closest)
			return closest->Dir == Direction::Clockwise;

		return false;
	}

	Atlas::Atlas(const TTF& ttf)
		:
		m_Width(0), 
		m_Height(0), 
		m_AtlasTextures()
	{		
		for (uint32_t c = '1'; c <= '9'; c++)
		{
			const Glyf& glyf = ttf.glyfs[ttf.cmap[c]];

			GlyphTexture texture;

			float scale = 1.0f;

			texture.Width = (glyf.x_max - glyf.x_min + 1);
			texture.Height = (glyf.y_max - glyf.y_min + 1);
			texture.Buffer.resize(texture.Width * texture.Height, 0x00);

			Path path = GetGlyphPath(glyf);

			std::cout << "current char: " << c << std::endl;

			for (size_t y = 0; y < texture.Height; y++)
			{
				for (size_t x = 0; x < texture.Width; x++)
				{
					bool isInsidePath = IsPointInsidePath((x + glyf.x_min), (y + glyf.y_min), path);

					//texture.Buffer[x + (texture.Height - y - 1) * texture.Width] = isInsidePath ? 255 : 0;
					texture.Buffer[x + y * texture.Width] = isInsidePath ? 255 : 0;
				}
			}

			m_AtlasTextures.push_back(texture);
		}
	}

	uint32_t Atlas::GetWidth() const
	{
		return m_Width;
	}

	uint32_t Atlas::GetHeight() const
	{
		return m_Height;
	}

#if 0
	std::vector<Contour> GetGlyphContours(const Glyf& glyf)
	{
		std::vector<Contour> contours(glyf.number_of_contours);

		size_t contour_index = 0;
		for (size_t i = 0; i < glyf.value.coordinates.size(); i++)		
		{
			Contour& contour = contours[contour_index];
			const GlyfPoint& point = glyf.value.coordinates[i];

			if (contour.size() && contour.back().on_curve && point.on_curve)
			{
				const GlyfPoint& p1 = contour.points.back();
				const GlyfPoint& p2 = point;

				GlyfPoint dummyPoint;
				dummyPoint.on_curve = false;
				dummyPoint.x = (p1.x + p2.x) / 2;
				dummyPoint.y = (p1.y + p2.y) / 2;

				contour.points.push_back(dummyPoint);
			}
			else if (contour.points.size() && !contour.points.back().on_curve && !point.on_curve)
			{
				const GlyfPoint& p1 = contour.points.back();
				const GlyfPoint& p2 = point;

				GlyfPoint dummyPoint;
				dummyPoint.on_curve = true;
				dummyPoint.x = (p1.x + p2.x) / 2;
				dummyPoint.y = (p1.y + p2.y) / 2;

				contour.points.push_back(dummyPoint);
			}

			contour.points.push_back(point);

			if (i == glyf.value.end_pts_of_contours[contour_index])
			{
				if (contour.points.size() && contour.points.front().on_curve && point.on_curve)
				{
					const GlyfPoint& p1 = contour.points.front();
					const GlyfPoint& p2 = point;

					GlyfPoint dummyPoint;
					dummyPoint.on_curve = false;
					dummyPoint.x = (p1.x + p2.x) / 2;
					dummyPoint.y = (p1.y + p2.y) / 2;

					contour.points.push_back(dummyPoint);
					contour.points.push_back(contour.points.front());
				}

				contour_index++;
			}
				
		}

		return contours;
	}
#endif
}

