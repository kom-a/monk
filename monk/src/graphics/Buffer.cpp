#include "graphics/Buffer.h"

#include "utils/OpenGL.h"

#include "core/Log.h"

namespace monk::gfx
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
			case BufferLayout::AttribType::Float:  return 4 * 1;
			case BufferLayout::AttribType::Float2: return 4 * 2;
			case BufferLayout::AttribType::Float3: return 4 * 3;
			case BufferLayout::AttribType::Float4: return 4 * 4;
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
			case BufferLayout::AttribType::Float:  return GL_FLOAT;
			case BufferLayout::AttribType::Float2: return GL_FLOAT;
			case BufferLayout::AttribType::Float3: return GL_FLOAT;
			case BufferLayout::AttribType::Float4: return GL_FLOAT;
		}

		LOG_ERROR("Invalid attrib pointer type");
		return -1;
	}

	// Vertex Buffer ///////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(float* data, size_t size, const BufferLayout& layout)
		: m_Layout(layout)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
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

	void VertexBuffer::SetData(float* data, size_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	// Index Buffer ///////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(uint32_t* data, size_t count)
		: m_Count(count)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);
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

}