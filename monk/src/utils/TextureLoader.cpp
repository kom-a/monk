#include "TextureLoader.h"

#include "core/Assert.h"
#include "core/Log.h"

namespace monk
{
	//////////////////////////////////////////////////////////////////////////
	// Helper functions
	//////////////////////////////////////////////////////////////////////////

	static uint16_t ReadU16(uint8_t** data)
	{
		uint16_t value;
		memcpy(&value, *data, sizeof(uint16_t));
		*data += sizeof(uint16_t);
		return value;
	}

	static uint32_t ReadU32(uint8_t** data)
	{
		uint32_t value;
		memcpy(&value, *data, sizeof(uint32_t));
		*data += sizeof(uint32_t);
		return value;
	}

	//////////////////////////////////////////////////////////////////////////
	// Structure definitions
	//////////////////////////////////////////////////////////////////////////

	struct BitMapFileHeader
	{
		uint16_t Type;
		uint32_t Size;
		uint16_t Reserverd1;
		uint16_t Reserverd2;
		uint32_t OffsetBits;
	};

	struct BitMapInfoHeader
	{
		uint32_t Size;
		uint32_t Width;
		uint32_t Height;
		uint16_t Planes;
		uint16_t BitCount;
		uint32_t Compression;
		uint32_t SizeImage;
		uint32_t XpelsPerMeter;
		uint32_t YpelsPerMeter;
		uint32_t ColorsUsed;
		uint32_t ColorsImportant;
	};

	struct BMPImage
	{
		BitMapFileHeader FileHeader = { 0 };
		BitMapInfoHeader InfoHeader = { 0 };
		const uint32_t* ColorTable = nullptr;
		const uint8_t* Image = nullptr;

		BMPImage(uint8_t* data)
		{
			uint8_t* base = data;

			FileHeader.Type = ReadU16(&data);
			FileHeader.Size = ReadU32(&data);
			FileHeader.Reserverd1 = ReadU16(&data);
			FileHeader.Reserverd2 = ReadU16(&data);
			FileHeader.OffsetBits = ReadU32(&data);

			InfoHeader.Size = ReadU32(&data);
			InfoHeader.Width = ReadU32(&data);
			InfoHeader.Height = ReadU32(&data);
			InfoHeader.Planes = ReadU16(&data);
			InfoHeader.BitCount = ReadU16(&data);
			InfoHeader.Compression = ReadU32(&data);
			InfoHeader.SizeImage = ReadU32(&data);
			InfoHeader.XpelsPerMeter = ReadU32(&data);
			InfoHeader.YpelsPerMeter = ReadU32(&data);
			InfoHeader.ColorsUsed = ReadU32(&data);
			InfoHeader.ColorsImportant = ReadU32(&data);

			ColorTable = (uint32_t*)data;
			Image = base + FileHeader.OffsetBits;
		}
	};

	enum class BMPCompressionMethod
	{
		BI_RGB = 0,
		BI_RLE8 = 1,
		BI_RLE4 = 2,
		BI_BITFIELDS = 3,
		BI_JPEG = 4,
		BI_PNG = 5,
		BI_ALPHABITFIELDS = 6,
		BI_CMYK = 11,
		BI_CMYKRLE8 = 12,
		BI_CMYKRLE4 = 13,
	};

	//////////////////////////////////////////////////////////////////////////
	// Implementation
	//////////////////////////////////////////////////////////////////////////

	TextureData TextureLoader::LoadFromFile(const std::string& filename, TextureFormat format)
	{
		FileData filedata = FileManager::ReadBytes(filename);
		TextureData textureData;

		if (IsBMP(filedata))
			textureData = LoadBMP(filedata, format);
		else
			MONK_ASSERT("Unsupported texture file format");

		filedata.Free();

		return textureData;
	}

	bool TextureLoader::IsBMP(const FileData& filedata)
	{
		return filedata.Data[0] == 'B' && filedata.Data[1] == 'M';
	}

	TextureData TextureLoader::LoadBMP(const FileData& filedata, TextureFormat format)
	{
		BMPImage bmp(filedata.Data);

		MONK_ASSERT(bmp.InfoHeader.Compression == (uint32_t)BMPCompressionMethod::BI_RGB || true,
			"BMP compression method not supported");
		MONK_ASSERT(bmp.InfoHeader.ColorsUsed == 0,
			"BMP color pallete not supported");
		MONK_ASSERT(bmp.InfoHeader.Planes == 1,
			"BMP planes != 1 not supported");
		MONK_ASSERT(bmp.InfoHeader.BitCount == 24 || true,
			"BMP bits per pixel must be 24");

		TextureData texture;
		texture.Width = bmp.InfoHeader.Width;
		texture.Height = bmp.InfoHeader.Height;
		texture.Channels = TextureFormatBytesPerPixel(format);
		texture.Data = new uint8_t[(size_t)bmp.InfoHeader.Width * bmp.InfoHeader.Height * texture.Channels];

		size_t padding = (texture.Width % 4) == 0 ? 0 : 4 - (texture.Width % 4);
		int bpp = bmp.InfoHeader.BitCount / 8;

		for (int y = 0; y < texture.Height; y++)
		{
			int yy = texture.Height - 1 - y;
			for (int x = 0; x < texture.Width; x++)
			{
				int textureIndex = (y * texture.Width + x) * texture.Channels;
				int bmpIndex = (yy * texture.Width + x + padding * yy) * bpp;

				switch (format)
				{
					case TextureFormat::RGB:
					{
						texture.Data[textureIndex + 0] = bmp.Image[bmpIndex + 2];
						texture.Data[textureIndex + 1] = bmp.Image[bmpIndex + 1];
						texture.Data[textureIndex + 2] = bmp.Image[bmpIndex + 0];
					} break;
					case TextureFormat::RGBA:
					{
						texture.Data[textureIndex + 0] = bmp.Image[bmpIndex + 2];
						texture.Data[textureIndex + 1] = bmp.Image[bmpIndex + 1];
						texture.Data[textureIndex + 2] = bmp.Image[bmpIndex + 0];
						texture.Data[textureIndex + 3] = 255;
					} break;
				}
			}
		}

		return texture;
	}

	int TextureLoader::TextureFormatBytesPerPixel(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::RGB: return 3;
			case TextureFormat::RGBA: return 4;
		}

		LOG_ERROR("Unknown texture format");
		return 0;
	}

	void TextureData::Free()
	{
		delete[] Data;
	}
}