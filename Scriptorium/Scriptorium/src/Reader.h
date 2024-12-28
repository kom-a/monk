#pragma once

#include <inttypes.h>
#include <string>
#include <optional>
#include <vector>

#include "Common.h"

namespace Scriptorium
{
	class Reader
	{
	public:
		explicit Reader(const void* buffer, Endianness endianness = Endianness::LITTLE);
		~Reader() = default;

	public:
		const std::byte* ReadBuffer(size_t size);
		
		int8_t  ReadInt8 ();
		int16_t ReadInt16();
		int32_t ReadInt32();
		int64_t ReadInt64();

		uint8_t  ReadUInt8 ();
		uint16_t ReadUInt16();
		uint32_t ReadUInt32();
		uint64_t ReadUInt64();

		float ReadFloat();
		double ReadDouble();

		std::string ReadString(std::optional<size_t> length = std::nullopt);
		std::wstring ReadWString(std::optional<size_t> length = std::nullopt);

		void SetCursor(size_t cursor);
		size_t GetCursor() const;

		void PushCursor();
		void PushCursor(size_t newCursor);
		void PopCursor();

		void Skip(size_t offset);

		void SetEndianness(Endianness endianness);
	private:
		size_t m_Cursor;
		const uint8_t* m_Buffer;
		Endianness m_Endianness;

		std::vector<size_t> m_CursorStack;
	};

}