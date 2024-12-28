#pragma once

#include <inttypes.h>
#include <vector>
#include <initializer_list>

#include "../core/Memory.h"

namespace monk
{
	class BufferLayout
	{
	public:
		enum class AttribType
		{
			Float, Float2, Float3, Float4
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

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(VertexArray& other) = delete;
		VertexArray(VertexArray&& other) = delete;
		VertexArray& operator=(VertexArray& other) = delete;

		void Bind() const;
		void Unbind() const;

		static Ref<VertexArray> Create();
	private:
		uint32_t m_ID = (uint32_t)-1;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(float* data, size_t size, const BufferLayout& layout);
		VertexBuffer(size_t size, const BufferLayout& layout);
		~VertexBuffer();

		VertexBuffer(VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) = delete;
		VertexBuffer& operator=(VertexBuffer& other) = delete;

	public:
		void Bind() const;
		void Unbind() const;

		void* Map();
		void Unmap();

		static Ref<VertexBuffer> Create(float* data, size_t size, const BufferLayout& layout);
		static Ref<VertexBuffer> Create(size_t size, const BufferLayout& layout);

	private:
		uint32_t m_ID;
		BufferLayout m_Layout;
		size_t m_Size;
	};

	class IndexBuffer
	{
	public:
		enum class IndexType { NONE, UNSIGNED_SHORT, UNSIGNED_INT };
		enum class IndexUsage { NONE, STATIC, DYNAMIC };

		IndexBuffer(void* data, size_t count, IndexType indexType = IndexType::UNSIGNED_INT, IndexUsage indexUsage = IndexUsage::STATIC);
		IndexBuffer(size_t count, IndexType indexType);
		~IndexBuffer();

		IndexBuffer(IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) = delete;
		IndexBuffer& operator=(IndexBuffer& other) = delete;

	public:
		void Bind() const;
		void Unbind() const;

		uint32_t* Map();
		void Unmap();

		uint32_t Count() const { return m_Count; }
		IndexType Type() const { return m_IndexType; }
		
		static uint32_t ToOpenGLType(IndexType indexType);

		static Ref<IndexBuffer> Create(void* data, size_t count, IndexType indexType = IndexType::UNSIGNED_INT, IndexUsage indexUsage = IndexUsage::STATIC);
		static Ref<IndexBuffer> Create(size_t count, IndexType indexType);

	private:
		uint32_t IndexTypeToSize(IndexType indexType);
		uint32_t IndexUsageToOpenGLUsage(IndexUsage indexUsage);

	private:
		uint32_t m_ID = 0;
		uint32_t m_Count;
		IndexType m_IndexType;
	};
}