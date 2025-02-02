#include "MFL.h"

#include <fstream>
#include <iostream>
#include <optional>
#include <map>
#include <thread>

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

	const MFL::Glyf& TTF::GetGlyfByUnicode(uint32_t unicode) const
	{
		return glyfs[cmap[unicode]];
	}

	const MFL::GlyphMetrics TTF::GetGlyfMetricsByUnicode(uint32_t unicode) const
	{
		MFL::GlyphMetrics metrics;
		uint32_t index = cmap[unicode];

		if (index < hhea.number_of_hmetrics)
		{
			metrics.Advance = hmtx.hor_metrics[index].advanceWidth;
			metrics.LeftSizeBearing = hmtx.hor_metrics[index].leftSideBearing;
		}

		return metrics;
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
				reader.PushCursor(dirTableEntry.offset);
				head.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "cvt ")
			{
				reader.PushCursor(dirTableEntry.offset);
				cvt.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "prep")
			{
				reader.PushCursor(dirTableEntry.offset);
				prep.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "kern")
			{
				reader.PushCursor(dirTableEntry.offset);
				kern.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "hhea")
			{
				reader.PushCursor(dirTableEntry.offset);
				hhea.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "post")
			{
				reader.PushCursor(dirTableEntry.offset);
				post.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "OS/2")
			{
				reader.PushCursor(dirTableEntry.offset);
				os2.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "name")
			{
				reader.PushCursor(dirTableEntry.offset);
				name.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "maxp")
			{
				reader.PushCursor(dirTableEntry.offset);
				maxp.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "fpgm")
			{
				reader.PushCursor(dirTableEntry.offset);
				fpgm.Parse(reader);
				reader.PopCursor();
			}
			else if (dirTableEntry.tag == "cmap")
			{
				reader.PushCursor(dirTableEntry.offset);
				cmap.Parse(reader);
				reader.PopCursor();
			}
		}

		auto it_hmtx = m_TableLocation.find("hmtx");

		if (it_hmtx != m_TableLocation.end())
		{
			reader.PushCursor(it_hmtx->second);
			hmtx.Parse(reader, maxp.num_glyphs, hhea.number_of_hmetrics);
			reader.PopCursor();
		}

		size_t location_table_start = m_TableLocation["loca"];
		size_t glyf_table_start = m_TableLocation["glyf"];

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
				if (component.number_of_contours == -1)
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

	GlyfConstraints GetGlyphConstraints(const Glyf& glyf)
	{
		GlyfConstraints gc;
		gc.XMin = glyf.x_min;
		gc.XMax = glyf.x_max;
		gc.YMin = glyf.y_min;
		gc.YMax = glyf.y_max;

		return gc;
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

	struct Parabola
	{
		float AX;
		float AY;

		float BX;
		float BY;

		float CX;
		float CY;
	};

	inline static Parabola CalculateCoefficients(const BezierCurve& curve)
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

	static std::vector<Intersection> RayCurveIntersection(int x, int y, const BezierCurve& curve)
	{
		std::vector<Intersection> result;

		const GlyfPoint& p0 = curve.P0;
		const GlyfPoint& p1 = curve.P1;
		const GlyfPoint& p2 = curve.P2;

		if (p0.y < y && p1.y < y && p2.y < y)
			return {};
		if (p0.y > y && p1.y > y && p2.y > y)
			return {};

		Parabola p = CalculateCoefficients(curve);

		auto [t0, t1] = CalculateQuadraticRoots(p.AY, p.BY, p.CY - y);

		float off = 0;
		while (t0 == t1 && t0 != FLT_MAX)
		{
			off += p.AY >= 0 ? -1.0f : 1.0f;

			auto [tt0, tt1] = CalculateQuadraticRoots(p.AY, p.BY, p.CY - y + off);

			t0 = tt0;
			t1 = tt1;
		}

		float intersect0 = p.AX * t0 * t0 + p.BX * t0 + p.CX - x;
		float intersect1 = p.AX * t1 * t1 + p.BX * t1 + p.CX - x;

		const float eps = 0;
		bool valid0 = t0 >= -eps && t0 <= 1 + eps;
		bool valid1 = t1 >= -eps && t1 <= 1 + eps;

		if (valid0)
		{
			auto intersection = Intersection();
			intersection.T = t0;
			intersection.Distance = intersect0;
			intersection.GradX = 2 * p.AX * intersection.T + p.BX;
			intersection.GradY = 2 * p.AY * intersection.T + p.BY;

			intersection.Dir = intersection.GradY < 0 ? Direction::Clockwise : Direction::Counterclockwise;

			result.push_back(intersection);
		}

		if (valid1)
		{
			auto intersection = Intersection();
			intersection.T = t1;
			intersection.Distance = intersect1;

			intersection.GradX = 2 * p.AX * intersection.T + p.BX;
			intersection.GradY = 2 * p.AY * intersection.T + p.BY;

			intersection.Dir = intersection.GradY < 0 ? Direction::Clockwise : Direction::Counterclockwise;

			result.push_back(intersection);
		}

		return result;
	}

	static bool IsHoleContour(const Contour& contour)
	{
		int64_t area = 0;

		for (const BezierCurve& curve : contour)
			area += curve.P0.x * curve.P2.y - curve.P2.x * curve.P0.y;

		return area > 0;
	}

	ContourHitRecord RayContourIntersections(int x, int y, const Contour& contour)
	{
		ContourHitRecord result;

		bool isHole = IsHoleContour(contour);

		for (const auto& curve : contour)
		{
			ContourHitRecord record = RayCurveIntersection(x, y, curve);

			for (auto& intersection : record)
			{
				intersection.isHole = isHole;
				result.push_back(intersection);
			}
		}

		std::sort(result.begin(), result.end(), [](const Intersection& left, const Intersection& right) {

			if (right.Dir != left.Dir && std::abs(left.Distance - right.Distance) < 1e-5f)
			{
				float rightGradientDirection = right.T > 0.5f ? -1.0f : 1.0f;
				float leftGradientDirection = left.T > 0.5f ? -1.0f : 1.0f;

				float rightGradientLength = std::sqrt(right.GradX * right.GradX + right.GradY * right.GradY);
				float leftGradientLength = std::sqrt(left.GradX * left.GradX + left.GradY * left.GradY);

				float rightGradientX = (rightGradientDirection * right.GradX) / rightGradientLength;
				float leftGradientX = (leftGradientDirection * left.GradX) / leftGradientLength;

				if (leftGradientX < rightGradientX)
					return true;
			}

			return left.Distance < right.Distance;
			});

		return result;
	}

	PathHitRecord GetLineIntersections(size_t line, const Path& path, const GlyfConstraints& constraints)
	{
		PathHitRecord result(path.size());

		for (size_t i = 0; i < path.size(); i++)
		{
			const Contour& contour = path[i];

			result[i] = RayContourIntersections(constraints.XMin, line + constraints.YMin, contour);
		}

		return result;
	}

	static bool IsDistanceInsideContourHitRecord(size_t distance, const ContourHitRecord& record)
	{
		bool inside = false;

		std::optional<Intersection> left;
		std::optional<Intersection> right;

		for (size_t i = 0; i < record.size(); i++)
		{
			if (record[i].Distance > distance)
			{
				right = record[i];
				break;
			}

			left = record[i];
		}

		if (!left || !right)
			return false;

		if (left && right)
		{
			bool isHole = left->isHole;

			if (!isHole && left->Dir == Direction::Clockwise && right->Dir == Direction::Counterclockwise)
				return false;
		}

		return true;
	}

	static bool IsDistanceInsidePathHitRecord(size_t distance, const PathHitRecord& record)
	{
		bool inside = false;

		for (const ContourHitRecord& contourRecord : record)
		{
			bool insideContour = IsDistanceInsideContourHitRecord(distance, contourRecord);
			if (insideContour)
				inside = true;

			if (insideContour && contourRecord.size() && contourRecord.front().isHole)
				return false;
		}

		return inside;
	}

	static void ProcessPathHitRecord(size_t line, float scale, const PathHitRecord& record, GlyphTexture& texture)
	{
		if (!record.size())
			return;

		for (size_t i = 0; i < texture.Width; i++)
		{
			bool inside = IsDistanceInsidePathHitRecord(i / scale, record);

			const size_t bufferIndex = (texture.Height - line - 1) * texture.Width + i;
			texture.Buffer[bufferIndex] = inside ? 255 : 0;
		}

		return;
	}

	static void ProcessTextureLine(size_t line, float scale, const Path& path, const GlyfConstraints& constraints, GlyphTexture& texture)
	{
		PathHitRecord record = GetLineIntersections(line / scale, path, constraints);

		ProcessPathHitRecord(line, scale, record, texture);
	}

	Atlas::Atlas(const TTF& ttf)
		:
		m_Width(0),
		m_Height(0),
		m_GlyphTextures(),
		m_FontSize(256.0f),
		m_FontAscender(ttf.hhea.ascender),
		m_FontDescender(ttf.hhea.descender)
	{
		RasterizeGlyphs(ttf);
		CreateAtlas(m_GlyphTextures);
	}

	uint32_t Atlas::GetWidth() const
	{
		return m_Width;
	}

	uint32_t Atlas::GetHeight() const
	{
		return m_Height;
	}

	const uint8_t* Atlas::GetTexture() const
	{
		return m_Atlas.data();
	}

	float Atlas::GetFontSize() const
	{
		return m_FontSize;
	}

	GlyphData Atlas::GetGlyphData(uint32_t unicode) const
	{
		GlyphDataMap::const_iterator it = m_GlyphData.find(unicode);
		if (it != m_GlyphData.end())
			return it->second;

		GlyphDataMap::const_iterator it_null = m_GlyphData.find(0);
		if (it_null != m_GlyphData.end())
			return it_null->second;

		return GlyphData();
	}

	float Atlas::GetScaleForFontSize(uint32_t fontSize) const
	{
		return ((float)fontSize / (m_FontSize));
	}

	void Atlas::RasterizeWorker(GlyphTextureMap& atlasTextures, const TTF& ttf, uint32_t unicodeStart, uint32_t unicodeEnd) const
	{
		atlasTextures.reserve(unicodeEnd - unicodeStart);

		for (uint32_t c = unicodeStart; c < unicodeEnd; c++)
		{
			GlyphTexture texture = RasterizeGlyph(ttf, c);
			atlasTextures.try_emplace(c, std::move(texture));
		}
	}

	void Atlas::CreateAtlas(const GlyphTextureMap& atlasTextures)
	{
		AtlasGlyphMetrics metrics = CalculateAtlasGlyphMetrics(atlasTextures);
		uint32_t side = (uint32_t)std::ceil(std::sqrtf(metrics.SumArea));

		m_Width = side + metrics.MaxWidth * 3;
		m_Height = side + metrics.MaxHeight * 3;
		m_Atlas.resize(m_Width * m_Height);

		uint32_t maxRowHeight = 0;

		uint32_t offsetX = 0;
		uint32_t offsetY = 0;
		uint32_t gap = 20;

		for (const auto& [unicode, texture] : atlasTextures)
		{
			if (offsetX + texture.Width > m_Width)
			{
				offsetY += maxRowHeight + gap;
				maxRowHeight = 0;
				offsetX = 0;
			}
			
			if (texture.Height > maxRowHeight)
				maxRowHeight = texture.Height;

			GlyphData glyphData;
			glyphData.UV_BottomLeft		= { offsetX					, offsetY + texture.Height	};
			glyphData.UV_TopLeft		= { offsetX					, offsetY					};
			glyphData.UV_TopRight		= { offsetX + texture.Width	, offsetY					};
			glyphData.UV_BottomRight	= { offsetX + texture.Width	, offsetY + texture.Height	};
			glyphData.Width = texture.Width;
			glyphData.Height = texture.Height;
			m_GlyphData.try_emplace(unicode, glyphData);

			for (uint32_t y = 0; y < texture.Height; y++)
			{
				for(uint32_t x = 0; x < texture.Width; x++)
				{
					m_Atlas[(y + offsetY) * m_Width + x + offsetX] = texture.Buffer[y * texture.Width + x];
				}
			}

			offsetX += texture.Width + gap;
		}

		int qwe = 123;
	}

	Atlas::AtlasGlyphMetrics Atlas::CalculateAtlasGlyphMetrics(const GlyphTextureMap& atlasTextures)
	{
		AtlasGlyphMetrics metrics;

		for (const auto&[unicode, texture] : atlasTextures)
		{
			if (texture.Width > metrics.MaxWidth)
				metrics.MaxWidth = texture.Width;

			if (texture.Height > metrics.MaxHeight)
				metrics.MaxHeight = texture.Height;

			metrics.SumArea += texture.Width * texture.Height;
		}

		return metrics;
	}

	void Atlas::RasterizeGlyphs(const TTF& ttf)
	{
		uint32_t threadCount = std::thread::hardware_concurrency();
		std::vector<std::thread> threads;
		std::vector<GlyphTextureMap> maps(threadCount);

		size_t unicodeStartRange = '!';
		size_t unicodeEndRange = '~' + 1;
		size_t unicodeRange = unicodeEndRange - unicodeStartRange;

		size_t sizePerWorker = unicodeRange / threadCount;

		for (size_t t = 0; t < threadCount; t++)
		{
			size_t start = unicodeStartRange + t * sizePerWorker;
			size_t end = (t == threadCount - 1) ? unicodeEndRange : start + sizePerWorker;

			threads.emplace_back(&Atlas::RasterizeWorker, this, std::ref(maps[t]), ttf, start, end);
		}

		GlyphTexture nullTexture = RasterizeGlyphByIndex(ttf, 0);
		m_GlyphTextures.try_emplace(0, std::move(nullTexture));

		for (auto& thread : threads)
			thread.join();

		for (const GlyphTextureMap& m : maps)
			m_GlyphTextures.insert(m.begin(), m.end());
	}

	GlyphTexture Atlas::RasterizeGlyphByIndex(const TTF& ttf, uint32_t index) const
	{
		const Glyf& glyf = ttf.glyfs[index];
		const Path& path = GetGlyphPath(glyf);
		const GlyfConstraints& constraints = GetGlyphConstraints(glyf);

		const float scale = m_FontSize / (ttf.hhea.ascender - ttf.hhea.descender);

		std::cout << "index: " << index << std::endl;

		GlyphTexture texture(glyf, scale);

		for (size_t y = 0; y < texture.Height; y++)
			ProcessTextureLine(y, scale, path, constraints, texture);

		return texture;
	}

	GlyphTexture Atlas::RasterizeGlyph(const TTF& ttf, uint32_t unicode) const
	{
		return RasterizeGlyphByIndex(ttf, ttf.cmap[unicode]);
	}

	GlyphTexture::GlyphTexture()
		:
		Width(0),
		Height(0),
		Buffer()
	{ }

	GlyphTexture::GlyphTexture(GlyphTexture&& other) noexcept
		:
		Width(other.Width),
		Height(other.Height),
		Buffer(std::move(other.Buffer))
	{ }

	GlyphTexture& GlyphTexture::operator=(GlyphTexture&& other) noexcept
	{
		Width = other.Width;
		Height = other.Height;
		Buffer = std::move(other.Buffer);

		return *this;
	}

	GlyphTexture::GlyphTexture(const Glyf& glyf, float scale)
		:
		Width((glyf.x_max - glyf.x_min)* scale),
		Height((glyf.y_max - glyf.y_min)* scale),
		Buffer(Width* Height, 0x00)
	{ }
}

