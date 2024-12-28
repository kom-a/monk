#pragma once

#include "../core/Memory.h"
#include "Buffer.h"
#include "Shader.h"
#include <MML/MML.h>
#include "Model.h"
#include "Camera.h"

namespace monk
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Begin(Ref<Camera> camera, Ref<Shader> shader);
		void End();

		void DrawIndexed(const VertexBuffer& buffer, const IndexBuffer& indices);
		void DrawModel(Ref<Model> model);
	private:
		uint32_t m_VAO = 0;
		Ref<Shader> m_Shader = nullptr;
		Ref<Camera> m_Camera = nullptr;
	};
}