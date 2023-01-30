#pragma once

#include <inttypes.h>
#include <vector>
#include <initializer_list>

namespace monk
{
	class BufferLayout
	{
	public:
		enum class AttribType
		{
			Float, Float2, Float3, Float4,
		};

		struct Attrib
		{
			unsigned Index;
			AttribType Type;

			int Size() const;
			int ComponentCount() const;
			int ToOpenGLType() const;
		};

	public:
		BufferLayout() = default;
		BufferLayout(std::initializer_list<Attrib> layout);

	private:
		friend class VertexBuffer;
		void Bind() const;
	private:
		size_t m_Stride;
		std::vector<Attrib> m_Layout;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(float* data, size_t size, const BufferLayout& layout);
		VertexBuffer(size_t size, const BufferLayout& layout);
		~VertexBuffer();

	public:
		void Bind() const;
		void Unbind() const;

		void* Map();
		void Unmap();

	private:
		uint32_t m_ID;
		BufferLayout m_Layout;
		size_t m_Size;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* data, size_t count);
		IndexBuffer(size_t count);
		~IndexBuffer();

	public:
		void Bind() const;
		void Unbind() const;

		uint32_t* Map();
		void Unmap();

		uint32_t Count() const { return m_Count; }

	private:
		uint32_t m_ID;
		uint32_t m_Count;
	};
}