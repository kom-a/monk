#include "graphics/Buffer.h"

#include <MOGL/MOGL.h>

#include "core/Log.h"
#include "core/Assert.h"

namespace monk
{
	// Buffer Layout ///////////////////////////////////////////////////

	BufferLayout::BufferLayout(std::initializer_list<Attrib> layout)
		: m_Layout(layout), m_Stride(0)
	{
		for (const auto& l : m_Layout)
			m_Stride += l.Size();
	}

	void BufferLayout::Bind() const
	{
		int offset = 0;
		for (const auto& l : m_Layout)
		{
			glVertexAttribPointer(
				l.Index,
				l.ComponentCount(),
				l.ToOpenGLType(),
				GL_FALSE,
				m_Stride,
				(const void*)offset);
			glEnableVertexAttribArray(l.Index);

			offset += l.Size();
		}
	}

	int BufferLayout::Attrib::Size() const
	{
		switch (Type)
		{
			case BufferLayout::AttribType::Float	: return 4 * 1;
			case BufferLayout::AttribType::Float2	: return 4 * 2;
			case BufferLayout::AttribType::Float3	: return 4 * 3;
			case BufferLayout::AttribType::Float4	: return 4 * 4;
		}
	}

	int BufferLayout::Attrib::ComponentCount() const
	{
		switch (Type)
		{
		case BufferLayout::AttribType::Float:  return 1;
		case BufferLayout::AttribType::Float2: return 2;
		case BufferLayout::AttribType::Float3: return 3;
		case BufferLayout::AttribType::Float4: return 4;
		}
	}

	int BufferLayout::Attrib::ToOpenGLType() const
	{
		switch (Type)
		{
			case BufferLayout::AttribType::Float: 
			case BufferLayout::AttribType::Float2:
			case BufferLayout::AttribType::Float3:
			case BufferLayout::AttribType::Float4: return GL_FLOAT;
		}

		LOG_ERROR("Invalid attrib pointer type");
		return -1;
	}

	// Vertex Array ///////////////////////////////////////////////////

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_ID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_ID);
		m_ID = (uint32_t)-1;
	}

	void VertexArray::Bind() const 
	{
		glBindVertexArray(m_ID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<VertexArray>();
	}

	// Vertex Buffer ///////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(float* data, size_t size, const BufferLayout& layout)
		: m_Size(size), m_Layout(layout)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(size_t size, const BufferLayout& layout)
		: m_Size(size), m_Layout(layout)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
		m_ID = (uint32_t) -1;
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		m_Layout.Bind();
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void* VertexBuffer::Map()
	{
		Bind();
		return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void VertexBuffer::Unmap()
	{
		Bind();
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* data, size_t size, const BufferLayout& layout)
	{
		return CreateRef<VertexBuffer>(data, size, layout);
	}

	Ref<VertexBuffer> VertexBuffer::Create(size_t size, const BufferLayout& layout)
	{
		return CreateRef<VertexBuffer>(size, layout);
	}

	// Index Buffer ///////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(void* data, size_t count, IndexType indexType, IndexUsage indexUsage)
		: m_Count(count), m_IndexType(indexType)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexTypeToSize(indexType) * count, data, IndexUsageToOpenGLUsage(indexUsage));
	}

	IndexBuffer::IndexBuffer(size_t count, IndexType indexType)
		: m_Count(count), m_IndexType(indexType)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexTypeToSize(indexType) * count, nullptr, GL_DYNAMIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t* IndexBuffer::Map()
	{
		Bind();
		uint32_t* data = (uint32_t*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_WRITE);
		return data;
	}

	void IndexBuffer::Unmap()
	{
		Bind();
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

	uint32_t IndexBuffer::ToOpenGLType(IndexType indexType)
	{
		switch (indexType)
		{
		case IndexType::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
		case IndexType::UNSIGNED_INT: return GL_UNSIGNED_INT;
		default: LOG_ERROR("Unknown index type: {0}", (int)indexType);
		}

		return 0;
	}

	Ref<IndexBuffer> IndexBuffer::Create(void* data, size_t count, IndexType indexType /*= IndexType::UNSIGNED_INT*/, IndexUsage indexUsage /*= IndexUsage::STATIC*/)
	{
		return CreateRef<IndexBuffer>(data, count, indexType, indexUsage);
	}

	Ref<IndexBuffer> IndexBuffer::Create(size_t count, IndexType indexType)
	{
		return CreateRef<IndexBuffer>(count, indexType);
	}

	uint32_t IndexBuffer::IndexTypeToSize(IndexType indexType)
	{
		switch (indexType)
		{
			case IndexType::UNSIGNED_SHORT: return sizeof(unsigned short);
			case IndexType::UNSIGNED_INT: return sizeof(unsigned int);
			default: LOG_ERROR("Unknown index type: {0}", (int)indexType);
		}

		return 0;
	}

	uint32_t IndexBuffer::IndexUsageToOpenGLUsage(IndexUsage indexUsage)
	{
		switch (indexUsage)
		{
			case IndexUsage::STATIC: return GL_STATIC_DRAW;
			case IndexUsage::DYNAMIC: return GL_DYNAMIC_DRAW;
			default: LOG_ERROR("Unknown index usage: {0}", (int)indexUsage);
		}

		return 0;
	}
}