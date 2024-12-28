#include "Renderer.h"

#include "core/Assert.h"
#include <MOGL/MOGL.h>
#include "utils/FileManager.h"

namespace monk
{
	Renderer::Renderer()
	{
		glGenVertexArrays(1, &m_VAO);
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Renderer::Begin(Ref<Camera> camera, Ref<Shader> shader)
	{
		MONK_ASSERT(!m_Camera, "Call Renderer::End first");
		MONK_ASSERT(!m_Shader, "Call Renderer::End first");

		m_Camera = camera;
		m_Shader = shader;
	}

	void Renderer::End()
	{
		MONK_ASSERT(m_Camera, "Call Renderer::Begin first");
		MONK_ASSERT(m_Shader, "Call Renderer::Begin first");

		m_Shader = nullptr;
		m_Camera = nullptr;
	}

	void Renderer::DrawIndexed(const VertexBuffer& buffer, const IndexBuffer& indices)
	{
		glBindVertexArray(m_VAO);
		buffer.Bind();
		indices.Bind();
		m_Shader->Bind();

		glDrawElements(GL_TRIANGLES, indices.Count(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawModel(Ref<Model> model)
	{
		MONK_ASSERT(m_Shader, "Call Renderer::Begin first");
		MONK_ASSERT(m_Camera, "Call Renderer::Begin first");

		glBindVertexArray(m_VAO);

		m_Shader->Bind();
		m_Shader->SetMatrix4("u_ProjectionView", m_Camera->GetProjectionViewMatrix());

		for (const auto& mesh : model->m_Meshes)
		{
			mesh.m_VertexBuffer->Bind();
			mesh.m_IndexBuffer->Bind();
			m_Shader->SetMatrix4("u_Model", mesh.m_ModelMatrix);
			glDrawElements(GL_TRIANGLES, mesh.m_IndexBuffer->Count(), IndexBuffer::ToOpenGLType(mesh.m_IndexBuffer->Type()), nullptr);
		}
	}
}
