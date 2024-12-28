#pragma once

#include <filesystem>
#include <unordered_map>

#include <Scriptorium/Scriptorium.h>

#include "Types.h"

#include <optional>

namespace MFL
{
	class TTF
	{
	public:
		TTF(const std::filesystem::path& filename);

		OffsetTable offsetTable;
		
		Head	head;
		Cvt		cvt;
		Prep	prep;
		Kern	kern;
		Hhea	hhea;
		Post	post;
		Os2		os2;
		Name	name;
		Maxp	maxp;
		Fpgm	fpgm;
		Cmap	cmap;
		Loca	loca;

		std::vector<Glyf> 	glyfs;

	private:
		void Parse(Scriptorium::Reader& reader);

	private:
		std::unordered_map<std::string, size_t> m_TableLocation;
	};

	class Atlas
	{
	public:
		Atlas(const TTF& ttf);
		~Atlas() = default;

		uint32_t GetWidth	() const;
		uint32_t GetHeight	() const;
		uint8_t* GetTexture	() const;
	public:
		struct GlyphTexture
		{
			uint32_t Width;
			uint32_t Height;
			std::vector<uint8_t> Buffer;
		};

		uint32_t m_Width;
		uint32_t m_Height;
		std::vector<GlyphTexture> m_AtlasTextures;
	};

	class Font
	{
	public:
		Font(const std::filesystem::path& filename);
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

	using Contour	= std::vector<BezierCurve>;
	using Path		= std::vector<Contour>;	

	Path GetGlyphPath(const Glyf& glyf);

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
		float Distance = 0.0f;
		float T = 0.0f;
		Direction Dir = Direction::None;

		GlyfPoint P0;
		GlyfPoint P1;
		GlyfPoint P2;

		int x, y;
	};

	std::optional<Intersection> RayCurveIntersection(int x, int y, const BezierCurve& curve);
	std::optional<Intersection> RayContourIntersection(int x, int y, const Contour& contour);

	// END DEBUG

	//std::vector<Contour> GetGlyphContours(const Glyf& glyf);
	//std::vector<BezierCurve> GetGlyphCurves(const Glyf& glyf);
	//std::vector<GlyfPoint> GetGlyphPoints(const Glyf& glyf);
}
