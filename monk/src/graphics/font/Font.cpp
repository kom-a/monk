#include "Font.h"

#include "utils/OpenGL.h"
#include "utils/FileManager.h"
#include "core/Log.h"

//#define STB_TRUETYPE_IMPLEMENTATION
//#include "graphics/font/stb_truetype.h"

#include <filesystem>

namespace monk
{
	float Font::s_SdfSize = 32;
	
	Font::Font(const std::string& filename)
	{
		utils::FileData ttf = utils::FileManager::ReadBytes(filename);
		Truetype tt(ttf.Data);

		CreateFontAtlas();

		uint8_t* atlasData = new uint8_t[m_FontAtlas.Size * m_FontAtlas.Size];

		for (int ch = 32; ch < 127; ++ch) 
		{
			float scale = tt.ScaleForPixelHeight(s_SdfSize);
			SdfChar fc = tt.GetCodepointSDF(scale, ch, m_Padding, m_Onedge, m_PixelDistScale);

			m_FontChars[ch].Width = fc.Width;
			m_FontChars[ch].Height = fc.Height;
			m_FontChars[ch].XOffset = fc.XOffset;
			m_FontChars[ch].YOffset = fc.YOffset;
			m_FontChars[ch].Advance = fc.Advance;

			int atlasX = ch % m_FontAtlas.CharsInRow;
			int atlasY = ch / m_FontAtlas.CharsInRow;
			int charSize = m_FontAtlas.Size / m_FontAtlas.CharsInRow;

			int px = atlasX * charSize;
			int py = atlasY * charSize;

			m_FontChars[ch].P0.x = (float)px / m_FontAtlas.Size;
			m_FontChars[ch].P0.y = (float)py / m_FontAtlas.Size;
			m_FontChars[ch].P1.x = (float)(px + fc.Width) / m_FontAtlas.Size;
			m_FontChars[ch].P1.y = (float)(py + fc.Height) / m_FontAtlas.Size;

			AppendCharData(atlasData, fc, atlasX, atlasY);

			fc.Free();
		}

		UploadFontAtlas(atlasData);

		delete[] atlasData;
		ttf.Free();
	}

	void Font::CreateFontAtlas()
	{
		m_FontAtlas.CharsInRow = std::ceil(std::sqrt(FONT_ATLAST_CHAR_COUNT));
		m_FontAtlas.Size = m_FontAtlas.CharsInRow * (s_SdfSize + m_Padding * 2);

		glGenTextures(1, &m_FontAtlas.TextureID);
		glBindTexture(GL_TEXTURE_2D, m_FontAtlas.TextureID);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_FontAtlas.Size, m_FontAtlas.Size, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void Font::UploadFontAtlas(uint8_t* data)
	{
		glBindTexture(GL_TEXTURE_2D, m_FontAtlas.TextureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_FontAtlas.Size, m_FontAtlas.Size, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void Font::AppendCharData(uint8_t* atlasData, const SdfChar& fc, int x, int y)
	{
		int charSize = m_FontAtlas.Size / m_FontAtlas.CharsInRow;

		int px = charSize * x;
		int py = charSize * y;

		for (int i = 0; i < fc.Height; i++)
		{
			int yy = i + py;
			for (int j = 0; j < fc.Width; j++)
			{
				int xx = px + j;
				atlasData[yy * m_FontAtlas.Size + xx] = fc.Data[i * fc.Width + j];
			}
		}
	}

	Font::~Font()
	{
		glDeleteTextures(1, &m_FontAtlas.TextureID);
	}
}
