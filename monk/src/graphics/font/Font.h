#pragma once

#include "stb_truetype.h"
#include "math/Math.h"
#include "graphics/font/Truetype.h"

#include <string>

#define FONT_ATLAST_CHAR_COUNT 128

namespace monk
{
	struct FontChar
	{
		math::vec2 P0; // Character top-left coordinates on font atlas
		math::vec2 P1; // Character bottom-right coordinates on font atlas

		int Width;
		int Height;
		int XOffset;
		int YOffset;
		float Advance;
	};

	class Font
	{
	public:
		Font(const std::string& filename);
		~Font();

		FontChar GetChar(int character) { return m_FontChars[character]; }
		uint32_t GetAtlasTextureID() { return m_FontAtlas.TextureID; }

		static float GetSdfSize() { return s_SdfSize; }

	private:
		void CreateFontAtlas();
		void UploadFontAtlas(uint8_t* data);
		void AppendCharData(uint8_t* atlasData, const SdfChar& fc, int x, int y);

	private:
		struct FontAtlas {
			uint32_t TextureID = 0;
			uint32_t CharsInRow = 0;
			uint32_t Size = 0;
		} m_FontAtlas;

		FontChar m_FontChars[FONT_ATLAST_CHAR_COUNT] = { 0 };
		const float m_PixelDistScale = 128.0f;
		const int m_Onedge = 128;
		const int m_Padding = 3;

		static float s_SdfSize;

	};
}
