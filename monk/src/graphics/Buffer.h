#pragma once

#include <inttypes.h>

namespace monk::gfx
{
	class VertexBuffer
	{
	public:
		VertexBuffer(float* data, size_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_ID;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* data, size_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		uint32_t m_ID;
	};
}