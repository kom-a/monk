#include "TextureLoader.h"

#include <cctype>

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

	static uint32_t ReadASCII(uint8_t** data)
	{
		uint32_t value = 0;
		while (std::isdigit(**data))
		{
			int digit = **data - '0';
			value *= 10;
			value += digit;
			*data += 1;
		}

		return value;
	}

	static void PNM_EatWhitespaces(uint8_t** data)
	{
		const char* whitespace = " \t\n\r";
		while (std::strchr(whitespace, **data) && **data != '\0')
			*data += 1;
	}

	static void PNM_EatSingleWhitespace(uint8_t** data)
	{
		const char* whitespace = " \t\n\r";
		if (std::strchr(whitespace, **data) && **data != '\0')
			*data += 1;
		else
			MONK_ASSERT("Not a whitespace character");
	}

	static void PNM_EatSingleComment(uint8_t** data)
	{
		if (**data == '#')
			while (**data != '\n')
				*data += 1;
		*data += 1;
	}

	static void PNM_EatComments(uint8_t** data)
	{
		while (**data == '#')
		{
			PNM_EatSingleComment(data);
		}
	}

	static void PNM_Next(uint8_t** data)
	{
		PNM_EatWhitespaces(data);
		while (**data == '#')
		{
			PNM_EatSingleComment(data);
			PNM_EatWhitespaces(data);
		}
	}

	static bool PNM_IsWhitespace(uint8_t* data)
	{
		const char* whitespace = " \t\n\r";
		return std::strchr(whitespace, *data) && *data != '\0';
	}

	static std::string PNM_Token(uint8_t** data)
	{
		std::string token;

		while (!PNM_IsWhitespace(*data))
		{
			token.append(1, **data);
			*data += 1;
		}

		return token;
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
		int32_t Width;
		int32_t Height;
		uint16_t Planes;
		uint16_t BitCount;
		uint32_t Compression;
		uint32_t SizeImage;
		int32_t XpelsPerMeter;
		int32_t YpelsPerMeter;
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

	struct PPMImage
	{
		uint8_t P6[2] = {0};
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Maxval = 0;
		uint8_t* Image = nullptr;

		PPMImage(uint8_t* data)
		{
			P6[0] = data[0];
			P6[1] = data[1];
			data += 2;

			PNM_Next(&data);
			Width = ReadASCII(&data);

			PNM_Next(&data);
			Height = ReadASCII(&data);

			PNM_Next(&data);
			Maxval = ReadASCII(&data);

			PNM_Next(&data); // TOOD: This could not work if image data starts with "whitespace" character (???)
			Image = data;

			if (Maxval != 255)
				MONK_ASSERT("Not supported");
		}
	};

	struct PAMImage
	{
		uint8_t P7[2] = {0};
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t MaxVal = 0;
		uint32_t Depth = 0;
		enum class PNMFormat {
			NONE,
			RGB,
			RGB_ALPHA
		} Format = PNMFormat::NONE;
		uint8_t* Image = nullptr;

		PAMImage(uint8_t* data)
		{
			P7[0] = data[0];
			P7[1] = data[1];
			data += 2;

			std::string token;
			PNM_Next(&data);

			// TODO: Error handling
			do
			{
				token = PNM_Token(&data);
				PNM_EatWhitespaces(&data);
				if (token == "WIDTH")
					Width = ReadASCII(&data);
				else if (token == "HEIGHT")
					Height = ReadASCII(&data);
				else if (token == "DEPTH")
					Depth = ReadASCII(&data);
				else if (token == "MAXVAL")
					MaxVal = ReadASCII(&data);
				else if (token == "TUPLTYPE")
				{
					PNM_EatWhitespaces(&data);
					token = PNM_Token(&data);
					if (token == "RGB")
						Format = PNMFormat::RGB;
					else if (token == "RGB_ALPHA")
						Format = PNMFormat::RGB_ALPHA;
					else
						MONK_ASSERT("Unsupported PNM format");
				}
				else if (token == "ENDHDR")
				{
					// Pass
				}
				else
				{
					MONK_ASSERT("Unexpected token");
				}
				PNM_Next(&data);
			} while (token != "ENDHDR");

			Image = data;

			MONK_ASSERT(MaxVal == 255, "Unsupported");
		}
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
		else if (IsPPM(filedata))
			textureData = LoadPPM(filedata, format);
		else if (IsPAM(filedata))
			textureData = LoadPAM(filedata, format);
		else
			MONK_ASSERT("Unsupported texture file format");

		filedata.Free();

		return textureData;
	}

	bool TextureLoader::IsBMP(const FileData& filedata)
	{
		return filedata.Data[0] == 'B' && filedata.Data[1] == 'M';
	}

	bool TextureLoader::IsPPM(const FileData& filedata)
	{
		return filedata.Data[0] == 'P' && filedata.Data[1] == '6';
	}

	bool TextureLoader::IsPAM(const FileData& filedata)
	{
		return filedata.Data[0] == 'P' && filedata.Data[1] == '7';
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

	TextureData TextureLoader::LoadPPM(const FileData& filedata, TextureFormat format)
	{
		MONK_ASSERT(format == TextureFormat::INTERNAL, "Unsupported");

		PPMImage ppm(filedata.Data);

		TextureData texture;
		texture.Width = ppm.Width;
		texture.Height = ppm.Height;
		texture.Channels = 3; // TODO: Make this not hardcoded
		texture.Data = new uint8_t[(size_t)texture.Width * texture.Height * texture.Channels];
		memcpy(texture.Data, ppm.Image, texture.Width * texture.Height * texture.Channels);

		return texture;
	}

	TextureData TextureLoader::LoadPAM(const FileData& filedata, TextureFormat format)
	{
		MONK_ASSERT(format == TextureFormat::INTERNAL, "Unsupported");

		PAMImage pam(filedata.Data);

		TextureData texture;
		texture.Width = pam.Width;
		texture.Height = pam.Height;
		switch (pam.Format)
		{
		case PAMImage::PNMFormat::RGB: texture.Channels = 3; break;
		case PAMImage::PNMFormat::RGB_ALPHA: texture.Channels = 4; break;
		default: MONK_ASSERT("Unsupported pam format");
		}
		texture.Data = new uint8_t[(size_t)texture.Width * texture.Height * texture.Channels];
		memcpy(texture.Data, pam.Image, texture.Width * texture.Height * texture.Channels);

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