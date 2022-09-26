#pragma once

#include "stb_truetype.h"
#include "math/Math.h"

#include <string>

namespace monk
{
	struct Glyph
	{
		float x0, y0, s0, t0;
		float x1, y1, s1, t1;
	};

	class Font
	{
	public:
		Font(const std::string& filename);
		~Font();

		void Bind() const;
		void Unbind() const;

		Glyph GetGlyph(char c, float* x, float* y) const;


	private:
		stbtt_bakedchar m_Cdata[96]; // ASCII 32..126 is 95 glyphs
		uint32_t m_Texture;

	};
}