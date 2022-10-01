#include "Font.h"

#include "utils/OpenGL.h"
#include "utils/FileManager.h"
#include "core/Log.h"

//#define STB_TRUETYPE_IMPLEMENTATION
//#include "graphics/font/stb_truetype.h"

#include <filesystem>

namespace monk
{
	
	Font::Font(const std::string& filename)
	{
		utils::FileData ttf = utils::FileManager::ReadBytes(filename);
		Truetype tt(ttf.Data);

		for (int ch = 32; ch < 127; ++ch) {
			float scale = tt.ScaleForPixelHeight(m_SdfSize);
			FontChar fc = tt.GetCodepointSDF(scale, ch, m_Padding, m_Onedge, m_PixelDistScale);
			m_FontChars[ch].Char = fc;

			uint32_t texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			// set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Load and generate the texture
			if (fc.Data)
			{
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fc.Width, fc.Height, 0, GL_RED, GL_UNSIGNED_BYTE, fc.Data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
				LOG_ERROR("Failed to load font texture for char: {0}", ch);

			m_FontChars[ch].TextureID = texture;
			m_FontChars[ch].SdfSize = m_SdfSize;
		}

		ttf.Free();
	}

	Font::~Font()
	{
		// TODO: delete generated textures
	}

}
