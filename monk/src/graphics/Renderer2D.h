#pragma once

#include "math/Math.h"
#include "graphics/Buffer.h"
#include "graphics/Shader.h"
#include "graphics/font/Font.h"

namespace monk
{
	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D();

		void Begin(const math::mat4& projection);
		void End();

		void Text(math::vec2 position, const std::string& text, int fontSize, const math::vec4& color = math::vec4(1.0f));

		void DrawRect(const math::vec2& position, const math::vec2& size, const math::vec4& color);
		void DrawRoundRect(const math::vec2& position, const math::vec2& size, const math::vec4& color, math::vec4 round = math::vec4(1.0f));
		void DrawCircle(const math::vec2& center, float radius, const math::vec4& color, uint32_t segments = 24);
		
		void FillRect(const math::vec2& position, const math::vec2& size, const math::vec4& color);
		void FillRoundRect(const math::vec2& position, const math::vec2& size, const math::vec4& color, math::vec4 round = math::vec4(1.0f));
		void FillCircle(const math::vec2& center, float radius, const math::vec4& color, uint32_t segments = 24);

		void DrawTexture(const math::vec2& position, const math::vec2& size, uint32_t textureID);

	private:
		unsigned m_VAO = 0;
		math::mat4 m_ProjectionMatrix = math::mat4(0);

		VertexBuffer* m_VertexBuffer = nullptr;
		IndexBuffer* m_IndexBuffer = nullptr;
		Shader* m_Shader = nullptr;

		// Font rendering
		// TODO: share font across all renderers (there is no need to load same font for every single renderer)
		Font* m_Font = nullptr;
		Shader* m_FontShader = nullptr;

		Shader* m_TextureShader = nullptr;
	};
}