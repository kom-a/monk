#pragma once

#include <string>
#include <inttypes.h>

#include "math/Math.h"

namespace monk
{
	class Texture2D
	{
	public:
		Texture2D(const std::string& filename);
		Texture2D(int width, int height, uint8_t* data);
		~Texture2D();

	public:
		void Bind(int unit) const;
		void Unbind();

	public:
		inline math::vec2 Size() const { return math::vec2(m_Width, m_Height); }

	private:
		unsigned m_TextureID;
		int m_Width, m_Height, m_Channels;
	};
}
