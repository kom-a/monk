#include "Texture2D.h"

#include "core/Log.h"
#include "core/Assert.h"
#include <MOGL/MOGL.h>
#include "utils/TextureLoader.h"

#include <fstream>

namespace monk
{
	Shared<Texture2D> Texture2D::Create(const std::string& filename, TextureFormat format)
	{
		TextureData textureData = TextureLoader::LoadFromFile(filename, format);
		auto result = CreateShared<Texture2D>(textureData.Width, textureData.Height, textureData.Channels, textureData.Data);
		textureData.Free();
		return result;
	}

	Shared<Texture2D> Texture2D::Create(int width, int height, TextureFormat format, uint8_t* data)
	{
		MONK_ASSERT(format != TextureFormat::INTERNAL, "Must specify texture format");
		return CreateShared<Texture2D>(width, height, TextureLoader::TextureFormatBytesPerPixel(format), data);
	}

	Texture2D::Texture2D(int width, int height, int channels, uint8_t* data)
		: m_Width(width), m_Height(height), m_Channels(channels)
	{
		MONK_ASSERT(channels == 3 || channels == 4, "Unsupported channels value");

		glGenTextures(1, &m_TextureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int format = channels == 4 ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	Texture2D::~Texture2D()
	{
		m_TextureID = 0;
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture2D::Bind(int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture2D::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}