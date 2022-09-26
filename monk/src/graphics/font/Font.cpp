#include "Font.h"

#include "utils/OpenGL.h"
#include "utils/FileManager.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "graphics/font/stb_truetype.h"

#include <filesystem>

namespace monk
{
	Font::Font(const std::string& filename)
	{
		auto ttf = utils::FileManager::ReadBytes(filename);
		uint8_t* bitmap = new uint8_t[512 * 512];
		uint8_t* bitmap4 = new uint8_t[512 * 512 * 4];

		stbtt_BakeFontBitmap(ttf.data(), 0, 64.0, bitmap, 512, 512, 32, 96, m_Cdata);

		for (int i = 0; i < 512 * 512; i++)
		{
			bitmap4[i * 4 + 0] = 255;
			bitmap4[i * 4 + 1] = 255;
			bitmap4[i * 4 + 2] = 255;
			bitmap4[i * 4 + 3] = bitmap[i];
		}

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap4);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		delete[] bitmap;
		delete[] bitmap4;
	}

	Font::~Font()
	{
		glDeleteTextures(1, &m_Texture);
	}

	void Font::Bind() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
	}

	void Font::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Glyph Font::GetGlyph(char c, float* x, float* y) const
	{
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(m_Cdata, 512, 512, c - 32, x, y, &q, 1);

		return *(Glyph*)&q;
	}
}
