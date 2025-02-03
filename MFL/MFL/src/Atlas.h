#pragma once

#include "Ttf.h"

#include "../include/MFL/MFL.h"

namespace MFL
{
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

		using GlyphTextureMap = std::unordered_map<uint32_t, GlyphTexture>;
		using GlyphDataMap = std::unordered_map<uint32_t, GlyphData>;

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
	};
}