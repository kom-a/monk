#include "Texture2D.h"

#include "core/Log.h"
#include "core/Assert.h"
#include <MOGL/MOGL.h>
#include "utils/TextureLoader.h"

#include <fstream>

namespace monk
{
	static uint32_t TextureFormat2OpenGLInternalFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RED : return GL_RED;
			case TextureFormat::RGB : return GL_RGB;
			case TextureFormat::RGBA: return GL_RGBA;
			default					: return GL_RGBA;
		}
	}

	Ref<Texture2D> Texture2D::Create(int width, int height, TextureFormat format, const uint8_t* data)
	{
		MONK_ASSERT(format != TextureFormat::INTERNAL, "Must specify texture format");
		return CreateRef<Texture2D>(width, height, format, data);
	}

	Texture2D::Texture2D(int width, int height, TextureFormat format, const uint8_t* data)
		: m_Width(width), m_Height(height), m_Format(format)
	{
		//MONK_ASSERT(channels == 3 || channels == 4, "Unsupported channels value");

		glGenTextures(1, &m_TextureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		uint32_t glFormat = TextureFormat2OpenGLInternalFormat(format);

		glTexImage2D(GL_TEXTURE_2D, 0, glFormat, m_Width, m_Height, 0, glFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = 0;
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