#pragma once

#include <inttypes.h>
#include <memory>

#include "IndexBuffer.h"
#include "Math.h"

namespace mui
{
	struct Vertex
	{
		Vec2f Position	= Vec2f();
		Vec4f Color		= Vec4f();
		Vec2f UV		= Vec2f();
		
		Vec2f Center	= Vec2f(-1.0f, -1.0f);
		float Radius	= -1.0f;

		Vertex(Vec2f position, Vec4f color, Vec2f uv) 
			: Position(position), Color(color), UV(uv)
		{ }

		Vertex(Vec2f position, Vec4f color, Vec2f center, float radius)
			: Position(position), Color(color), Center(center), Radius(radius)
		{ }
	};

	class VertexBuffer
	{
	public:
		VertexBuffer(size_t quadsCount);
		~VertexBuffer();

		VertexBuffer(VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) = delete;
		VertexBuffer& operator=(VertexBuffer& other) = delete;
		VertexBuffer& operator=(VertexBuffer&& other) = delete;

		void Bind();
		void SetData(const Vertex* data, size_t count);

	private:
		uint32_t m_ID = (uint32_t)-1;

		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_VertexArray = (uint32_t)-1;
	};
}