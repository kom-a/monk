#pragma once

#include <inttypes.h>

namespace mui
{
	class IndexBuffer
	{
	public:
		IndexBuffer(size_t quadsCount);
		~IndexBuffer();

		IndexBuffer(IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) = delete;
		IndexBuffer& operator=(IndexBuffer& other) = delete;
		IndexBuffer& operator=(IndexBuffer&& other) = delete;

		void Bind();
		static void Unbind();
	private:
		uint32_t m_ID = (uint32_t)-1;
	};
}