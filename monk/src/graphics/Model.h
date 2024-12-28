#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <MML/MML.h>

#include "../core/Memory.h"
#include "Buffer.h"

namespace monk
{
	class Mesh
	{
	public:
		Mesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, const mml::mat4& modelMatrix, const std::string& name = "None");
		~Mesh() = default;

	private:
		friend class Renderer;
		Ref<VertexBuffer> m_VertexBuffer = nullptr;
		Ref<IndexBuffer> m_IndexBuffer = nullptr;
		mml::mat4 m_ModelMatrix = mml::mat4(1.0f);
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