#pragma once

#include <string>
#include <inttypes.h>
#include <memory>

#include <MML/MML.h>

#include "../core/Memory.h"
#include "../utils/TextureLoader.h"

namespace monk
{
	enum class TextureFormat
	{
		INTERNAL,
		RED,
		RGB,
		RGBA,
	};

	class Texture2D
	{
	public:
		Texture2D(int width, int height, TextureFormat format, const uint8_t* data);
		~Texture2D();
		
		void Bind(int unit) const;
		void Unbind();

		inline mml::vec2 Size() const { return mml::vec2(m_Width, m_Height); }
		inline unsigned GetID() const { return m_TextureID; }

		static Ref<Texture2D> Create(int width, int height, TextureFormat format = TextureFormat::RGBA, const uint8_t* data = nullptr);

	private:
		unsigned m_TextureID = (unsigned)-1;
		int m_Width = 0;
		int m_Height = 0;
		TextureFormat m_Format;
	};
}
