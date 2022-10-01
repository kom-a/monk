#pragma once

#include "stb_truetype.h"
#include "math/Math.h"
#include "graphics/font/Truetype.h"

#include <string>

namespace monk
{
	struct FontData
	{
		FontChar Char;
		uint32_t TextureID;
		float SdfSize;
	};

	class Font
	{
	public:
		Font(const std::string& filename);
		~Font();

		FontData GetCharData(int character) { return m_FontChars[character]; }

	private:
		FontData m_FontChars[128] = { 0 };
		const float m_SdfSize = 128.0f;
		const float m_PixelDistScale = 256.0f;
		const int m_Onedge = 128;
		const int m_Padding = 3;
	};
}