#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "graphics/Buffer.h"
#include "core/Memory.h"
#include <MMath/MMath.h>

namespace monk
{
	class Mesh
	{
	public:
		Mesh(Shared<VertexBuffer> vertexBuffer, Shared<IndexBuffer> indexBuffer, const mmath::mat4& modelMatrix, const std::string& name = "None");
		~Mesh() = default;

	private:
		friend class Renderer;
		Shared<VertexBuffer> m_VertexBuffer = nullptr;
		Shared<IndexBuffer> m_IndexBuffer = nullptr;
		mmath::mat4 m_ModelMatrix = mmath::mat4(1.0f);
		std::string m_Name = "";
	};

	class Model
	{
	public:
		Model();

	private:
		friend class ModelLoader;
		friend class Renderer;
		std::vector<Mesh> m_Meshes;
	};
}