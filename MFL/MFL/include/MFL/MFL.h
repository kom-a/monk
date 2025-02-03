#pragma once

#include <filesystem>

namespace MFL
{
	struct UV
	{
		float U = 0.0f;
		float V = 0.0f;
	};

	struct GlyphData
	{
		UV UV_BottomLeft	= { 0.0f, 0.0f };
		UV UV_TopLeft		= { 0.0f, 0.0f };
		UV UV_TopRight		= { 0.0f, 0.0f };
		UV UV_BottomRight	= { 0.0f, 0.0f };

		uint32_t Width		= 0;
		uint32_t Height		= 0;

		float Ascender		= 0.0f;
		float Descender		= 0.0f;

		float Advance		= 0.0f;
	};

	class FontImpl_;

	class Font
	{
	public:
		explicit Font(const std::filesystem::path& filename);
		explicit Font(const uint8_t* ttfBuffer);
		~Font();

		uint32_t GetAtlasWidth() const;
		uint32_t GetAtlasHeight() const;
		const uint8_t* GetAtlasTextureBuffer() const;
		GlyphData GetGlyphDataByUnicode(uint32_t unicode) const;
		float GetScaleForFontSize(float fontSize) const;

	private:
		FontImpl_* Impl;
	};
}
