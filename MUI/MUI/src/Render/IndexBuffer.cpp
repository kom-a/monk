#include "IndexBuffer.h"

#include <MOGL/MOGL.h>

namespace mui
{
	IndexBuffer::IndexBuffer(size_t quadsCount)
	{
		size_t count = quadsCount * 6;
		uint32_t* data = new uint32_t[count];

		uint32_t quad_index = 0;
		const uint32_t pattern[6] = { 0, 1, 2, 2, 3, 0 };
		for (int i = 0; i < count; i++)
		{
			if (i != 0 && i % 6 == 0)
				quad_index += 4;

			data[i] = quad_index + pattern[i % 6];
		}

		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, data, GL_STATIC_DRAW);

		delete[] data;
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

