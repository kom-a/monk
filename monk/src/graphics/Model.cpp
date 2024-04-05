#include "Model.h"

#include <MOGL/MOGL.h>

namespace monk
{
	Model::Model()
	{

	}

	Mesh::Mesh(Shared<VertexBuffer> vertexBuffer, Shared<IndexBuffer> indexBuffer, const mmath::mat4& modelMatrix, const std::string& name)
		: m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_ModelMatrix(modelMatrix), m_Name(name)
	{
	}

}