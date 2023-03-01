#pragma once

#include <string>
#include <inttypes.h>

#include "utils/FileManager.h"
#include "core/Memory.h"

namespace monk
{
	struct TextureData
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Channels = 0;
		uint8_t* Data = nullptr;

		void Free();
	};

	enum class TextureFormat
	{
		INTERNAL,
		RGB,
		RGBA,
	};

	// Prefer not using BMP format for now (it is not always working)
	class TextureLoader
	{
	private:
		TextureLoader() = default;
		~TextureLoader() = default;
		
	public:
		static TextureData LoadFromFile(const std::string& filename, TextureFormat format);
		static int TextureFormatBytesPerPixel(TextureFormat format);

	private:
		static bool IsBMP(const FileData& filedata);
		static bool IsPPM(const FileData& filedata);
		static bool IsPAM(const FileData& filedata);

		static TextureData LoadBMP(const FileData& filedata, TextureFormat format);
		static TextureData LoadPPM(const FileData& filedata, TextureFormat format);
		static TextureData LoadPAM(const FileData& filedata, TextureFormat format);
	};
}