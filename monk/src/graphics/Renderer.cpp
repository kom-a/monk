#include "Renderer.h"

#include "utils/OpenGL.h"
#include "utils/FileManager.h"

namespace monk
{
	Renderer::Renderer()
	{
		glGenVertexArrays(1, &m_VAO);

		std::string vertexSrc = FileManager::ReadFile("res/Renderer.vert");
		std::string fragmentSrc = FileManager::ReadFile("res/Renderer.frag");
		m_Shader = new Shader(vertexSrc, fragmentSrc);
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Renderer::DrawIndexed(const VertexBuffer& buffer, const IndexBuffer& indices)
	{
		glBindVertexArray(m_VAO);
		buffer.Bind();
		indices.Bind();
		m_Shader->Bind();

		glDrawElements(GL_TRIANGLES, indices.Count(), GL_UNSIGNED_INT, nullptr);
	}

}