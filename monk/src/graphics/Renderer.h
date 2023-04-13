#pragma once

#include "core/Memory.h"
#include "Buffer.h"
#include "Shader.h"
#include "Math/Math.h"
#include "graphics/Model.h"
#include "graphics/Camera.h"

namespace monk
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Begin(Shared<Camera> camera, Shared<Shader> shader);
		void End();

		void DrawIndexed(const VertexBuffer& buffer, const IndexBuffer& indices);
		void DrawModel(Shared<Model> model);
	private:
		uint32_t m_VAO = 0;
		Shared<Shader> m_Shader = nullptr;
		Shared<Camera> m_Camera = nullptr;
	};
}