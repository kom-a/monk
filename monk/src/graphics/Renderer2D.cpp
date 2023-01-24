#include "graphics/Renderer2D.h"

#include "utils/OpenGL.h"
#include "utils/FileManager.h"
#include "core/Assert.h"

#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace monk
{
	Renderer2D::Renderer2D()
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

		std::string vertexSrc = utils::FileManager::ReadFile("res/VertexShader.glsl");
		std::string fragmentSrc = utils::FileManager::ReadFile("res/FragmentShader.glsl");

		m_Shader = new Shader(vertexSrc, fragmentSrc);

		vertexSrc = utils::FileManager::ReadFile("res/TextureVertex.glsl");
		fragmentSrc = utils::FileManager::ReadFile("res/TextureFragment.glsl");

		m_TextureShader = new Shader(vertexSrc, fragmentSrc);
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

	void Renderer2D::SetClearColor(const math::vec4& clearColor)
	{
		m_ClearColor = clearColor;
	}

	void Renderer2D::Clear()
	{
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer2D::FillRect(const math::vec2& position, const math::vec2& size, const math::vec4& color)
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

	void Renderer2D::FillRoundRect(const math::vec2& position, const math::vec2& size, const math::vec4& color, math::vec4 round /*= math::vec4(1.0f)*/)
	{
		std::vector<float> data;
		std::vector<uint32_t> indices;
		round = math::Clamp(round, math::vec4(0.0f), math::vec4(1.0f));
		math::vec4 radius = math::Min(size.x, size.y) * 0.25;

		const float coef = 20.0f; // TODO: Make this customizable
		radius[0] = math::Min(radius[0], coef * round[0]);
		radius[1] = math::Min(radius[1], coef * round[1]);
		radius[2] = math::Min(radius[2], coef * round[2]);
		radius[3] = math::Min(radius[3], coef * round[3]);

		// From top-left clockwise
		FillCircle(position + math::vec2(radius[0]), radius[0], color);
		FillCircle(position + math::vec2(size.x, 0.0f) + math::vec2(-radius[1], radius[1]), radius[1], color);
		FillCircle(position + math::vec2(size - radius[2]), radius[2], color);
		FillCircle(position + math::vec2(0.0f, size.y) + math::vec2(radius[3], -radius[3]), radius[3], color);

		// From top clockwise
		FillRect(position + math::vec2(radius[0], 0.0f), math::vec2(size.x - radius[0] - radius[1], math::Max(radius[0], radius[1])), color);
		FillRect(position + math::vec2(size.x - math::Max(radius[1], radius[2]), radius[1]), math::vec2(math::Max(radius[1], radius[2]), size.y - radius[1] - radius[2]), color);
		FillRect(position + math::vec2(radius[3], size.y - math::Max(radius[2], radius[3])), math::vec2(size.x - radius[2] - radius[3], math::Max(radius[2], radius[3])), color);
		FillRect(position + math::vec2(0.0f, radius[0]), math::vec2(math::Max(radius[0], radius[3]), size.y - radius[0] - radius[3]), color);

		// Center
		FillRect(position + math::vec2(radius[0]), size - radius[0] - radius[2], color); // NOTE: This doesn't work if neighbor radii are not equal to each other
	}

	void Renderer2D::FillCircle(const math::vec2& center, float radius, const math::vec4& color, uint32_t segments /*= 24*/)
	{
		// TODO: Optimize this to use same vertices for neighbor segments

		std::vector<float> data;
		std::vector<uint32_t> indices;

		float step = M_PI * 2.0f / segments;
		float angle = 0;

		data.push_back(center.x);
		data.push_back(center.y);
		data.push_back(0);
		data.push_back(color.r);
		data.push_back(color.g);
		data.push_back(color.b);
		data.push_back(color.a);

		for (int i = 0; i < segments; i++)
		{
			math::vec3 p1(std::cosf(angle), std::sinf(angle), 0.0f);
			angle += step;
			math::vec3 p2(std::cosf(angle), std::sinf(angle), 0.0f);
			
			p1 *= radius;
			p2 *= radius;

			data.push_back(p1.x + center.x);
			data.push_back(p1.y + center.y);
			data.push_back(p1.z);
			data.push_back(color.r);
			data.push_back(color.g);
			data.push_back(color.b);
			data.push_back(color.a);

			data.push_back(p2.x + center.x);
			data.push_back(p2.y + center.y);
			data.push_back(p2.z);
			data.push_back(color.r);
			data.push_back(color.g);
			data.push_back(color.b);
			data.push_back(color.a);

			indices.push_back(0);
			indices.push_back(i * 2 + 1);
			indices.push_back(i * 2 + 2);
		}

		IndexBuffer indexBuffer(&indices[0], indices.size());

		glBindVertexArray(m_VAO);
		m_VertexBuffer->Bind();
		m_VertexBuffer->SetData(&data[0], sizeof(float) * data.size());
		indexBuffer.Bind();

		m_Shader->Bind();
		m_Shader->SetMatrix4("u_Projection", m_ProjectionMatrix);

		glDrawElements(GL_TRIANGLES, indexBuffer.Count(), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
	}

	void Renderer2D::DrawTexture(const math::vec2& position, const math::vec2& size, uint32_t textureID)
	{
		float left = position.x;
		float right = position.x + size.x;
		float top = position.y;
		float bottom = position.y + size.y;

		float data[] = {
			left,	top,	0.0f, 0.0f,
			right,	top,	1.0f, 0.0f,
			right,	bottom, 1.0f, 1.0f,
			left,	bottom, 0.0f, 1.0f,
		};

		BufferLayout layout = {
			{ 0, BufferLayout::AttribType::Float2 },
			{ 1, BufferLayout::AttribType::Float2 },
		};

		VertexBuffer buffer(data, sizeof(data), layout);

		glBindVertexArray(m_VAO);

		buffer.Bind();
		buffer.SetData(data, sizeof(data));
		m_IndexBuffer->Bind();

		m_TextureShader->Bind();
		m_TextureShader->SetMatrix4("u_Projection", m_ProjectionMatrix);

		glDrawElements(GL_TRIANGLES, m_IndexBuffer->Count(), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
	}
}
