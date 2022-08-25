#pragma once

#include "math/Math.h"
#include "graphics/Buffer.h"
#include "graphics/Shader.h"

namespace monk::gfx
{
	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D();

		void Begin(const math::mat4& projection);
		void End();

		void DrawRect(const math::vec2& position, const math::vec2& size, const math::vec4& color);
		void FillRect(const math::vec2& position, const math::vec2& size, const math::vec4& color);

	private:
		unsigned m_VAO;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
		Shader* m_Shader;
		math::mat4 m_ProjectionMatrix;
	};
}