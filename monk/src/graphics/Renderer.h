#pragma once

#include "Buffer.h"
#include "Shader.h"
#include "Math/Math.h"

namespace monk
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void DrawIndexed(const VertexBuffer& buffer, const IndexBuffer& indices);
	private:
		uint32_t m_VAO = 0;
		Shader* m_Shader;
	};
}