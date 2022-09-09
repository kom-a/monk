#pragma once

#include "math/Math.h"
#include "graphics/Buffer.h"
#include "graphics/Shader.h"

namespace monk
{
	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D();

		void Begin(const math::mat4& projection);
		void End();

		void DrawRect(const math::vec2& position, const math::vec2& size, const math::vec4& color);
		void DrawRoundRect(const math::vec2& position, const math::vec2& size, const math::vec4& color, math::vec4 round = math::vec4(1.0f));
		void DrawCircle(const math::vec2& center, float radius, const math::vec4& color, uint32_t segments = 24);
		
		void FillRect(const math::vec2& position, const math::vec2& size, const math::vec4& color);
		void FillRoundRect(const math::vec2& position, const math::vec2& size, const math::vec4& color, math::vec4 round = math::vec4(1.0f));
		void FillCircle(const math::vec2& center, float radius, const math::vec4& color, uint32_t segments = 24);

	private:
		unsigned m_VAO;
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;
		Shader* m_Shader;
		math::mat4 m_ProjectionMatrix;
	};
}