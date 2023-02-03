#pragma once

#include <string>
#include <inttypes.h>
#include <memory>

#include "core/Memory.h"
#include "math/Math.h"
#include "utils/TextureLoader.h"

namespace monk
{
	class Texture2D
	{
	public:
		static Ref<Texture2D> Create(const std::string& filename, TextureFormat format = TextureFormat::RGBA);
		static Ref<Texture2D> Create(int width, int height, TextureFormat format = TextureFormat::RGBA, uint8_t* data = nullptr);


		Texture2D(int width, int height, int channels, uint8_t* data);
		~Texture2D();
		
	public:
		void Bind(int unit) const;
		void Unbind();

	public:
		inline math::vec2 Size() const { return math::vec2(m_Width, m_Height); }
		inline unsigned GetID() const { return m_TextureID; }

	private:
		unsigned m_TextureID = (unsigned)-1;
		int m_Width = 0, m_Height = 0, m_Channels = 0;
	};
}
