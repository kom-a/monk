#include "../include/MFL/MFL.h"

#include <fstream>
#include <iostream>
#include <optional>
#include <map>
#include <thread>

#include "Log.h"
#include "Ttf.h"
#include "Atlas.h"

namespace MFL
{
	class FontImpl_
	{
	public:
		explicit FontImpl_(const std::filesystem::path& filename);
		explicit FontImpl_(const uint8_t* ttfBuffer);

		uint32_t GetAtlasWidth() const;
		uint32_t GetAtlasHeight() const;
		const uint8_t* GetAtlasTextureBuffer() const;
		GlyphData GetGlyphDataByUnicode(uint32_t unicode) const;
		float GetScaleForFontSize(float fontSize) const;
		VerticalMetrics GetVerticalMetrics() const;

	private:
		TTF m_Ttf;
		Atlas m_Atlas;
	};

	FontImpl_::FontImpl_(const std::filesystem::path& filename)
		: 
		m_Ttf(filename),
		m_Atlas(m_Ttf)
	{ }

	FontImpl_::FontImpl_(const uint8_t* ttfBuffer)
		:
		m_Ttf(ttfBuffer),
		m_Atlas(m_Ttf)
	{ }

	uint32_t FontImpl_::GetAtlasWidth() const
	{
		return m_Atlas.GetWidth();
	}

	uint32_t FontImpl_::GetAtlasHeight() const
	{
		return m_Atlas.GetHeight();
	}

	const uint8_t* FontImpl_::GetAtlasTextureBuffer() const
	{
		return m_Atlas.GetTexture();
	}

	GlyphData FontImpl_::GetGlyphDataByUnicode(uint32_t unicode) const
	{
		GlyphData glyphData = m_Atlas.GetGlyphData(unicode);
		TTF::GlyphMetrics glyphMetrics = m_Ttf.GetGlyfMetricsByUnicode(unicode);
		const Glyf& glyf = m_Ttf.glyfs[m_Ttf.cmap[unicode]];

		float ttfScale = m_Atlas.GetFontSize() / (m_Ttf.hhea.ascender - m_Ttf.hhea.descender);

		glyphData.Ascender = glyf.y_max * ttfScale;
		glyphData.Descender = glyf.y_min * ttfScale;
		glyphData.Advance = glyphMetrics.Advance * ttfScale;

		return glyphData;
	}

	float FontImpl_::GetScaleForFontSize(float fontSize) const
	{
		return fontSize / m_Atlas.GetFontSize();
	}

	VerticalMetrics FontImpl_::GetVerticalMetrics() const
	{
		float ttfScale = m_Atlas.GetFontSize() / (m_Ttf.hhea.ascender - m_Ttf.hhea.descender);

		return {
			(float)(m_Ttf.hhea.ascender - m_Ttf.hhea.descender) * ttfScale,
			(float)m_Ttf.hhea.line_gap * ttfScale
		};
	}

	Font::Font(const std::filesystem::path& filename)
		:
		Impl(new FontImpl_(filename))
	{ }

	Font::Font(const uint8_t* ttfBuffer)
		:
		Impl(new FontImpl_(ttfBuffer))
	{ }

	Font::~Font()
	{
		delete Impl;
	}

	uint32_t Font::GetAtlasWidth() const
	{
		return Impl->GetAtlasWidth();
	}

	uint32_t Font::GetAtlasHeight() const
	{
		return Impl->GetAtlasHeight();
	}

	const uint8_t* Font::GetAtlasTextureBuffer() const
	{
		return Impl->GetAtlasTextureBuffer();
	}

	GlyphData Font::GetGlyphDataByUnicode(uint32_t unicode) const
	{
		return Impl->GetGlyphDataByUnicode(unicode);
	}

	float Font::GetScaleForFontSize(float fontSize) const
	{
		return Impl->GetScaleForFontSize(fontSize);
	}	

	VerticalMetrics Font::GetVerticalMetrics() const
	{
		return Impl->GetVerticalMetrics();
	}
}
