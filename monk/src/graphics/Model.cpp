#include "Model.h"

#include "utils/OpenGL.h"

namespace monk
{
	Model::Model()
	{

	}

	Mesh::Mesh(Shared<VertexBuffer> vertexBuffer, Shared<IndexBuffer> indexBuffer, const math::mat4& modelMatrix, const std::string& name)
		: m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_ModelMatrix(modelMatrix), m_Name(name)
	{
	}

}