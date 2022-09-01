#include "graphics/Renderer2D.h"

#include "utils/OpenGL.h"
#include "utils/FileManager.h"

#include <string>

namespace monk
{
	Renderer2D::Renderer2D()
		: m_ProjectionMatrix(math::mat4(1.0f))
	{
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		BufferLayout layout = {
			{ 0, BufferLayout::AttribType::Float3 },
			{ 1, BufferLayout::AttribType::Float4 },
		};

		m_VertexBuffer = new VertexBuffer(nullptr, 0, layout);
		
		uint32_t indices[] = {
			0, 1, 2, 0, 2, 3
		};

		m_IndexBuffer = new IndexBuffer(indices, 6);

		glBindVertexArray(0);

		std::string vertexSrc = utils::FileManager::ReadFile("res/vertexShader.glsl");
		std::string fragmentSrc = utils::FileManager::ReadFile("res/fragmentShader.glsl");

		m_Shader = new Shader(vertexSrc, fragmentSrc);
	}

	Renderer2D::~Renderer2D()
	{
		delete m_VertexBuffer;
		delete m_IndexBuffer;
		delete m_Shader;
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Renderer2D::Begin(const math::mat4& projection)
	{
		m_ProjectionMatrix = projection;
	}

	void Renderer2D::End()
	{

	}

	void Renderer2D::DrawRect(const math::vec2& position, const math::vec2& size, const math::vec4& color)
	{
		float left = position.x;
		float right = position.x + size.x;
		float top = position.y;
		float bottom = position.y + size.y;

		float data[] = {
			left,	top,	0.0f,		color.r, color.g, color.b, color.a,
			right,	top,	0.0f,		color.r, color.g, color.b, color.a,
			right,	bottom, 0.0f,		color.r, color.g, color.b, color.a,
			left,	bottom, 0.0f,		color.r, color.g, color.b, color.a,
		};

		glBindVertexArray(m_VAO);

		m_VertexBuffer->Bind();
		m_VertexBuffer->SetData(data, sizeof(data));
		m_IndexBuffer->Bind();

		m_Shader->Bind();
		m_Shader->SetMatrix4("u_Projection", m_ProjectionMatrix);

		glDrawElements(GL_TRIANGLES, m_IndexBuffer->Count(), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
	}

	void Renderer2D::FillRect(const math::vec2& position, const math::vec2& size, const math::vec4& color)
	{

	}
}
