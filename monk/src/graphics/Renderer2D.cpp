#include "graphics/Renderer2D.h"

#include <MOGL/MOGL.h>
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

		uint32_t whiteTexture = 0xffffffff;
		m_WhiteTexture = CreateUnique<Texture2D>(1, 1, 4, (uint8_t*)&whiteTexture);

		m_BatchStats.Textures = std::vector<uint32_t>();
	}

	Renderer2D::~Renderer2D()
	{
		delete m_VertexBuffer;
		delete m_IndexBuffer;
		delete m_Shader;
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Renderer2D::Begin(const OrthographicCamera& camera)
	{
		m_Camera = &camera;
		m_RenderStats.Reset();
		BeginBatch();
	}

	void Renderer2D::End()
	{
		EndBatch();
		Flush();
	}

	void Renderer2D::DrawQuad(const mmath::vec2& position, const mmath::vec2& size, const mmath::vec4& color)
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
		topLeftVertex.Position = mmath::vec3(position.x, position.y, 0.0f);
		topLeftVertex.Color = color;
		topLeftVertex.TextureID = 0;
		topLeftVertex.TextureCoords = mmath::vec2(0.0f);

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

	void Renderer2D::DrawTexture(const mmath::vec2& position, const mmath::vec2& size, const Texture2D& texture)
	{
		MONK_ASSERT(m_VertexBufferData);

		glBindVertexArray(m_VAO);

		if (m_BatchStats.Quads >= m_BatchSettings.MaxQuads || m_BatchStats.TextureIndex >= m_BatchSettings.MaxTextures - 1)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		float textureID = -1;
		for (int i = 0; i < m_BatchStats.TextureIndex; i++)
		{
			if (texture.GetID() == m_BatchStats.Textures[i])
			{
				textureID = i;
				break;
			}
		}

		if(textureID == -1)
		{
			m_BatchStats.Textures[m_BatchStats.TextureIndex] = texture.GetID();
			textureID = (float)m_BatchStats.TextureIndex;
			m_BatchStats.TextureIndex++;
		}

		Vertex& topLeftVertex = m_VertexBufferData[m_BatchStats.Verticies + 0];
		topLeftVertex.Position = mmath::vec3(position.x, position.y, 0.0f);
		topLeftVertex.Color = mmath::vec4(1.0f);
		topLeftVertex.TextureID = textureID;
		topLeftVertex.TextureCoords = mmath::vec2(0.0f, 0.0f);

		Vertex& topRightVertex = m_VertexBufferData[m_BatchStats.Verticies + 1];
		topRightVertex = topLeftVertex;
		topRightVertex.Position.x += size.x;
		topRightVertex.TextureCoords = mmath::vec2(1.0f, 0.0f);

		Vertex& bottomRightVertex = m_VertexBufferData[m_BatchStats.Verticies + 2];
		bottomRightVertex = topRightVertex;
		bottomRightVertex.Position.y += size.y;
		bottomRightVertex.TextureCoords = mmath::vec2(1.0f, 1.0f);

		Vertex& bottomLeftVertex = m_VertexBufferData[m_BatchStats.Verticies + 3];
		bottomLeftVertex = topLeftVertex;
		bottomLeftVertex.Position.y += size.y;
		bottomLeftVertex.TextureCoords = mmath::vec2(0.0f, 1.0f);

		m_BatchStats.Verticies += 4;
		m_BatchStats.Indices += 6;
		m_BatchStats.Quads += 1;
	}

	void Renderer2D::BeginBatch()
	{
		m_VertexBufferData = (Vertex*)m_VertexBuffer->Map();
		m_BatchStats.Reset();
		m_BatchStats.Textures.resize(m_BatchSettings.MaxTextures);
		m_BatchStats.Textures[0] = m_WhiteTexture->GetID();
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
		m_Shader->SetMatrix4("u_ProjectionView", m_Camera->GetProjectionViewMatrix());

		for (int i = 0; i < m_BatchStats.TextureIndex; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_BatchStats.Textures[i]);
			m_Shader->SetInt(std::string("u_Texture") + std::to_string(i), i);
		}

		glDrawElements(GL_TRIANGLES, m_BatchStats.Indices, GL_UNSIGNED_INT, nullptr);
	}
}
