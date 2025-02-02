#pragma once

#include <filesystem>
#include <unordered_map>

#include <Scriptorium/Scriptorium.h>

#include "Types.h"

#include <optional>

namespace MFL
{
	struct GlyphMetrics
	{
		uint32_t Advance			= 0;
		uint32_t LeftSizeBearing	= 0;
	};

	struct GlyphTexture
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		std::vector<uint8_t> Buffer;

		GlyphTexture();
		GlyphTexture(const GlyphTexture& other) = default;
		GlyphTexture(GlyphTexture&& other) noexcept;
		GlyphTexture& operator=(const GlyphTexture& other) = default;
		GlyphTexture& operator=(GlyphTexture&& other) noexcept;

		explicit GlyphTexture(const Glyf& glyf, float scale);
	};

	struct UV
	{
		uint32_t U = 0;
		uint32_t V = 0;
	};

	struct GlyphData
	{
		UV UV_BottomLeft;
		UV UV_TopLeft;
		UV UV_TopRight;
		UV UV_BottomRight;

		uint32_t Width;
		uint32_t Height;
	};

	class TTF
	{
	public:
		explicit TTF(const std::filesystem::path& filename);

		OffsetTable offsetTable;
		
		Head	head;
		Cvt		cvt;
		Prep	prep;
		Kern	kern;
		Hhea	hhea;
		Hmtx	hmtx;
		Post	post;
		Os2		os2;
		Name	name;
		Maxp	maxp;
		Fpgm	fpgm;
		Cmap	cmap;
		Loca	loca;

		std::vector<Glyf> 	glyfs;

	public:
		const Glyf& GetGlyfByUnicode(uint32_t unicode) const;
		const GlyphMetrics GetGlyfMetricsByUnicode(uint32_t unicode) const;

	private:
		void Parse(Scriptorium::Reader& reader);

	private:
		std::unordered_map<std::string, size_t> m_TableLocation;
	};

	class Atlas
	{
	public:
		explicit Atlas(const TTF& ttf);
		~Atlas() = default;

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		const uint8_t* GetTexture() const;
		float GetFontSize() const;

		GlyphData GetGlyphData(uint32_t unicode) const;
		float GetScaleForFontSize(uint32_t fontSize) const;

		using GlyphTextureMap	= std::unordered_map<uint32_t, GlyphTexture>;
		using GlyphDataMap		= std::unordered_map<uint32_t, GlyphData>;

	private:
		struct AtlasGlyphMetrics
		{
			uint32_t MaxWidth = 0;
			uint32_t MaxHeight = 0;

			float SumArea = 0.0f;
		};

	private:
		void RasterizeGlyphs(const TTF& ttf);
		GlyphTexture RasterizeGlyphByIndex(const TTF& ttf, uint32_t index) const;
		GlyphTexture RasterizeGlyph(const TTF& ttf, uint32_t unicode) const;

		void RasterizeWorker(GlyphTextureMap& atlasTextures, const TTF& ttf, uint32_t unicodeStart, uint32_t unicodeEnd) const;

		void CreateAtlas(const GlyphTextureMap& atlasTextures);

		AtlasGlyphMetrics CalculateAtlasGlyphMetrics(const GlyphTextureMap& atlasTextures);

	private:
		uint32_t m_Width;
		uint32_t m_Height;
		GlyphTextureMap m_GlyphTextures;
		float m_FontSize;

		std::vector<uint8_t>	m_Atlas;
		GlyphDataMap			m_GlyphData;

		uint32_t m_FontAscender;
		uint32_t m_FontDescender;
	};

	class Font
	{
	public:
		explicit Font(const std::filesystem::path& filename);
		~Font() = default;

		const TTF& GetTTF() const;
		const Atlas& GetAtlas() const;
		
	private:
		TTF m_Ttf;
		Atlas m_Atlas;
	};

	struct BezierCurve
	{
		GlyfPoint P0; 
		GlyfPoint P1;
		GlyfPoint P2;
	};

	struct GlyfConstraints
	{
		int32_t XMin;
		int32_t YMin;
		int32_t XMax;
		int32_t YMax;
	};

	// DEBUG 
	// TODO: Move to implementation file

	enum class Direction
	{
		None = 0,
		Clockwise,
		Counterclockwise
	};

	struct Intersection
	{
		float Distance = 0;
		float T = 0;
		Direction Dir = Direction::None;

		float GradX = 0;
		float GradY = 0;

		bool isHole = false;
	};

	using ContourHitRecord	= std::vector<Intersection>;
	using PathHitRecord		= std::vector<ContourHitRecord>;
	using Contour			= std::vector<BezierCurve>;
	using Path				= std::vector<Contour>;

	Path GetGlyphPath(const Glyf& glyf);
	GlyfConstraints GetGlyphConstraints(const Glyf& glyf);
	PathHitRecord GetLineIntersections(size_t line, const Path& path, const GlyfConstraints& constraints);

	// END DEBUG
}
