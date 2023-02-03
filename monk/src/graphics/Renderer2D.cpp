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

		m_VertexBuffer = new VertexBuffer(sizeof(Vertex) * m_BatchSettings.MaxVerticies, Vertex::GetLayout());

		uint32_t* indices = new uint32_t[m_BatchSettings.MaxIndices];
		size_t offset = 0;
		for (int i = 0; i < m_BatchSettings.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IndexBuffer = new IndexBuffer(indices, m_BatchSettings.MaxIndices);

		glBindVertexArray(0);

		std::string vertexSrc = FileManager::ReadFile("res/Renderer2D.vert");
		std::string fragmentSrc = FileManager::ReadFile("res/Renderer2D.frag");
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
		m_RenderStats.Reset();
		BeginBatch();
	}

	void Renderer2D::End()
	{
		EndBatch();
		Flush();
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
		MONK_ASSERT(m_VertexBufferData);

		glBindVertexArray(m_VAO);

		if (m_BatchStats.Quads >= m_BatchSettings.MaxQuads)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		Vertex& topLeftVertex = m_VertexBufferData[m_BatchStats.Verticies + 0];
		topLeftVertex.Position = math::vec3(position.x, position.y, 0.0f);
		topLeftVertex.Color = color;
		topLeftVertex.TextureID = 0;
		topLeftVertex.TextureCoords = math::vec2(0.0f);

		Vertex& topRightVertex = m_VertexBufferData[m_BatchStats.Verticies + 1];
		topRightVertex = topLeftVertex;
		topRightVertex.Position.x += size.x;

		Vertex& bottomRightVertex = m_VertexBufferData[m_BatchStats.Verticies + 2];
		bottomRightVertex = topRightVertex;
		bottomRightVertex.Position.y += size.y;

		Vertex& bottomLeftVertex = m_VertexBufferData[m_BatchStats.Verticies + 3]; 
		bottomLeftVertex = topLeftVertex;
		bottomLeftVertex.Position.y += size.y;

		m_BatchStats.Verticies += 4;
		m_BatchStats.Indices += 6;
		m_BatchStats.Quads += 1;
	}

#if 0
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
#endif

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
		m_IndexBuffer->Bind();

		static std::string vertexSrc = FileManager::ReadFile("res/TextureVertex.glsl");
		static std::string fragmentSrc = FileManager::ReadFile("res/TextureFragment.glsl");
		static Shader textureShader(vertexSrc, fragmentSrc);

		textureShader.Bind();
		textureShader.SetMatrix4("u_Projection", m_ProjectionMatrix);
		textureShader.SetInt("tex", 1);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
	}

	void Renderer2D::BeginBatch()
	{
		m_VertexBufferData = (Vertex*)m_VertexBuffer->Map();
		m_BatchStats.Reset();
	}

	void Renderer2D::EndBatch()
	{
		m_VertexBuffer->Unmap();
		m_VertexBufferData = nullptr;
		m_RenderStats += m_BatchStats;
	}

	void Renderer2D::Flush()
	{
		MONK_ASSERT(!m_VertexBufferData, "Unmap vertex buffer first");

		glBindVertexArray(m_VAO);

		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();

		m_Shader->Bind();
		m_Shader->SetMatrix4("u_Projection", m_ProjectionMatrix);
		m_Shader->SetMatrix4("u_View", math::mat4(1.0f));


		glDrawElements(GL_TRIANGLES, m_BatchStats.Indices, GL_UNSIGNED_INT, nullptr);
	}
}
