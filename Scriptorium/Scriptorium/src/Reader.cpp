#include "Reader.h"

#include <assert.h>
#include <sstream>

namespace Scriptorium
{
	Reader::Reader(const void* buffer, Endianness endianness)
		: 
		m_Cursor(0),
		m_Buffer((const uint8_t*)buffer),
		m_Endianness(endianness)
	{
	}

	const std::byte* Reader::ReadBuffer(size_t size)
	{
		auto* buffer = reinterpret_cast<const std::byte*>(&m_Buffer[m_Cursor]);
		m_Cursor += size;

		return buffer;
	}

	int8_t Reader::ReadInt8()
	{
		return std::to_integer<int8_t>(*ReadBuffer(sizeof(int8_t)));
	}

	int16_t Reader::ReadInt16()
	{
		const uint64_t v0 = ReadInt8() & 0xFF;
		const uint64_t v1 = ReadInt8() & 0xFF;

		switch (m_Endianness)
		{
			case Endianness::LITTLE:	return (v1 << 8) | (v0 << 0);
			case Endianness::BIG:		return (v0 << 8) | (v1 << 0);
		}
	}

	int32_t Reader::ReadInt32()
	{
		const uint64_t v0 = ReadInt8() & 0xFF;
		const uint64_t v1 = ReadInt8() & 0xFF;
		const uint64_t v2 = ReadInt8() & 0xFF;
		const uint64_t v3 = ReadInt8() & 0xFF;

		switch (m_Endianness)
		{
			case Endianness::LITTLE:	return (v3 << 24) | (v2 << 16) | (v1 << 8) | (v0 << 0);
			case Endianness::BIG:		return (v0 << 24) | (v1 << 16) | (v2 << 8) | (v3 << 0);
		}
	}

	int64_t Reader::ReadInt64()
	{
		const uint64_t v0 = ReadInt8() & 0xFF;
		const uint64_t v1 = ReadInt8() & 0xFF;
		const uint64_t v2 = ReadInt8() & 0xFF;
		const uint64_t v3 = ReadInt8() & 0xFF;
		const uint64_t v4 = ReadInt8() & 0xFF;
		const uint64_t v5 = ReadInt8() & 0xFF;
		const uint64_t v6 = ReadInt8() & 0xFF;
		const uint64_t v7 = ReadInt8() & 0xFF;

		switch (m_Endianness)
		{
		case Endianness::LITTLE:	return (v7 << 56) | (v6 << 48) | (v5 << 40) | (v4 << 32) | (v3 << 24) | (v2 << 16) | (v1 << 8) | (v0 << 0);
		case Endianness::BIG:		return (v0 << 56) | (v1 << 48) | (v2 << 40) | (v3 << 32) | (v4 << 24) | (v5 << 16) | (v6 << 8) | (v7 << 0);
		}
	}

	uint8_t Reader::ReadUInt8()
	{
		return (uint8_t)ReadInt8();
	}

	uint16_t Reader::ReadUInt16()
	{
		return (uint16_t)ReadInt16();
	}

	uint32_t Reader::ReadUInt32()
	{
		return (uint32_t)ReadInt32();
	}

	uint64_t Reader::ReadUInt64()
	{
		return (uint64_t)ReadInt64();
	}

	float Reader::ReadFloat()
	{
		return (float)ReadUInt32();
	}

	double Reader::ReadDouble()
	{
		return (double)ReadUInt64();
	}

	std::string Reader::ReadString(std::optional<size_t> length /*= std::nullopt*/)
	{
		std::stringstream ss;

		size_t i = 0;
		while (true)
		{
			if (length && i >= *length)
				break;

			char c = (char)ReadInt8();
			if (c == '\0')
				break;

			ss << c;
			i++;
		}

		return ss.str();
	}

	std::wstring Reader::ReadWString(std::optional<size_t> length /*= std::nullopt*/)
	{
		std::wstringstream ss;

		size_t i = 0;
		while (true)
		{
			if (length && i >= *length)
				break;

			wchar_t c = (wchar_t)ReadInt16();
			if (c == '\0')
				break;

			ss << c;
			i++;
		}

		return ss.str();
	}

	void Reader::SetCursor(size_t cursor)
	{
		m_Cursor = cursor;
	}

	size_t Reader::GetCursor() const
	{
		return m_Cursor;
	}

	void Reader::PushCursor()
	{
		m_CursorStack.push_back(m_Cursor);
	}

	void Reader::PushCursor(size_t newCursor)
	{
		PushCursor();
		m_Cursor = newCursor;
	}

	void Reader::PopCursor()
	{
		assert(m_CursorStack.size());
		m_Cursor = m_CursorStack.back();
		m_CursorStack.pop_back();
	}

	void Reader::Skip(size_t offset)
	{
		m_Cursor += offset;
	}

}