#include "ModelLoader.h"

#include "core/Log.h"
#include "core/Assert.h"

namespace monk
{
	GLTF ModelLoader::LoadGLTF(const Filepath& filename)
	{
		Shared<const JSONNode> jsonPtr;

		try
		{
			jsonPtr = JSON::ParseFile(filename.string());
		}
		catch (std::logic_error e)
		{
			LOG_ERROR("Failed to parse glTF JSON file\nReason: {0}", e.what());
			MONK_ASSERT("Not glTF model file format not supported");
		}

		const JSONNode& json = *jsonPtr;
		GLTF gltf;
		gltf.Buffers = GetGLTFBuffers(json, filename);
		gltf.BufferViews = GetGLTFBufferViews(json);
		gltf.Accessros = GetGLTFAccessors(json);
		gltf.Meshes = GetGLTFMeshes(json);
		gltf.Nodes = GetGLTFNodes(json);
		gltf.Scenes = GetGLTFScenes(json);
		gltf.Scene = json.TryGetNumber("scene", -1);

		return gltf;
	}

	std::vector<GLTFScene> ModelLoader::GetGLTFScenes(const JSONNode& json)
	{
		std::vector<GLTFScene> gltfScenes;
		auto scenes = json["scenes"].GetList();

		for (auto scene : scenes)
		{
			GLTFScene gltfScene;
			gltfScene.Name = (*scene).TryGetString("name", "No name");
			gltfScene.Nodes = scene->Has("nodes") ? GetNumbersFromJSONList<uint32_t>((*scene)["nodes"].GetList()) : std::vector<uint32_t>();

			gltfScenes.push_back(gltfScene);
		}

		return gltfScenes;
	}

	std::vector<GLTFBuffer> ModelLoader::GetGLTFBuffers(const JSONNode& json, const Filepath& root)
	{
		std::vector<GLTFBuffer> gltfBuffers;
		auto buffersList = json["buffers"].GetList();

		for (auto buffer : buffersList)
		{
			GLTFBuffer gltfBuffer;
			gltfBuffer.ByteLength = (*buffer)["byteLength"].GetNumber();
			gltfBuffer.URI = root.parent_path() / (*buffer)["uri"].GetString();

			gltfBuffers.push_back(gltfBuffer);
		}

		return gltfBuffers;
	}

	std::vector<GLTFBufferView> ModelLoader::GetGLTFBufferViews(const JSONNode& json)
	{
		std::vector<GLTFBufferView> gltfBufferViews;
		auto bufferViewsList = json["bufferViews"].GetList();

		for (auto bufferView : bufferViewsList)
		{
			GLTFBufferView gltfBufferView;
			gltfBufferView.Buffer = (*bufferView)["buffer"].GetNumber();
			gltfBufferView.ByteLength = (*bufferView)["byteLength"].GetNumber();
			gltfBufferView.ByteOffset = (*bufferView).TryGetNumber("byteOffset", 0);
			gltfBufferView.ByteStride = (*bufferView).TryGetNumber("byteStride", 0);
			gltfBufferView.Name = (*bufferView).TryGetString("name", "No name");
			gltfBufferView.Target = (*bufferView).TryGetNumber("target", -1);

			gltfBufferViews.push_back(gltfBufferView);
		}

		return gltfBufferViews;
	}

	std::vector<GLTFAccessor> ModelLoader::GetGLTFAccessors(const JSONNode& json)
	{
		std::vector <GLTFAccessor> gltfAccessors;
		auto accessors = json["accessors"].GetList();

		for (auto accessor : accessors)
		{
			GLTFAccessor gltfAccessor;
			gltfAccessor.BufferView = (*accessor)["bufferView"].GetNumber();
			gltfAccessor.ByteOffset = (*accessor).TryGetNumber("byteOffset", 0);
			gltfAccessor.ComponentType = (*accessor)["componentType"].GetNumber();
			gltfAccessor.Count = (*accessor)["count"].GetNumber();
			//gltfAccessor.Min = 
			//gltfAccessor.Max =
			gltfAccessor.Type = GLTFAccessor::ConvertToAccessorType((*accessor)["type"].GetString());

			gltfAccessors.push_back(gltfAccessor);
		}

		return gltfAccessors;
	}

	std::vector<GLTFMesh> ModelLoader::GetGLTFMeshes(const JSONNode& json)
	{
		std::vector<GLTFMesh> gltfMeshes;
		auto meshes = json["meshes"].GetList();

		for (auto mesh : meshes)
		{
			GLTFMesh gltfMesh;
			gltfMesh.Name = (*mesh).TryGetString("name", "No name");
			gltfMesh.Primitives = GetGLTFPrimitives((*mesh)["primitives"].GetList());

			gltfMeshes.push_back(gltfMesh);
		}

		return gltfMeshes;
	}

	std::vector<GLTFNode> ModelLoader::GetGLTFNodes(const JSONNode& json)
	{
		std::vector<GLTFNode> gltfNodes;
		auto nodes = json["nodes"].GetList();

		for (auto node : nodes)
		{
			GLTFNode gltfNode;
			gltfNode.Camera = (*node).TryGetNumber("camera", -1);
			gltfNode.Children = node->Has("children") ? GetNumbersFromJSONList<uint32_t>((*node)["children"].GetList()) : std::vector<uint32_t>();

			if (node->Has("matrix"))
			{
				gltfNode.Matrix = GetMatrixFromJSONList((*node)["matrix"].GetList());
				gltfNode.UseMatrix = true;
			}
			else
			{
				// TODO: Get Translation, Rotation and Scale and use them
				gltfNode.Matrix = math::mat4(1.0f);
				gltfNode.UseMatrix = false;
			}
			gltfNode.Mesh = (*node).TryGetNumber("mesh", -1);
			gltfNode.Name = (*node).TryGetString("name", "No name");

			gltfNodes.push_back(gltfNode);
		}

		return gltfNodes;
	}

	std::vector<GLTFPrimitive> ModelLoader::GetGLTFPrimitives(const JSONList& list)
	{
		std::vector<GLTFPrimitive> gltfPrimitives;

		for (auto primitive : list)
		{
			GLTFPrimitive gltfPrimitive;
			gltfPrimitive.Attributes = GetGLTFAttributes((*primitive)["attributes"]);
			gltfPrimitive.Indices = (*primitive).TryGetNumber("indices", -1);
			gltfPrimitive.Material = (*primitive).TryGetNumber("material", -1);
			gltfPrimitive.Mode = (*primitive).TryGetNumber("mode", -1);

			gltfPrimitives.push_back(gltfPrimitive);
		}

		return gltfPrimitives;
	}

	GLTFAttributes ModelLoader::GetGLTFAttributes(const JSONNode& json)
	{
		GLTFAttributes attributes;
		attributes.Position = json.TryGetNumber("POSITION", -1);
		attributes.Normal = json.TryGetNumber("NORMAL", -1);
		attributes.TexCoord_0 = json.TryGetNumber("TEXCOORD_0", -1);

		return attributes;
	}

	std::vector<Mesh> ModelLoader::ProcessNode(const GLTF& gltf, uint32_t node, const std::vector<FileData>& buffers, const math::mat4& parentMatrix)
	{
		std::vector<Mesh> meshes;

		if (gltf.Nodes[node].Children.empty())
		{
			if (gltf.Nodes[node].Mesh == -1)
				return meshes;
			MONK_ASSERT(gltf.Nodes[node].Mesh != -1, "No mesh if GLFT node with no children");
			const GLTFMesh& gltfMesh = gltf.Meshes[gltf.Nodes[node].Mesh];
			//MONK_ASSERT(gltfMesh.Primitives.size() == 1, "Multipile primitives not supported");
			const GLTFPrimitive& gltfPrimimitive = gltfMesh.Primitives[0];
			MONK_ASSERT(gltfPrimimitive.Mode == 4, "GLTF Primitive mode != 4 not supported");
			MONK_ASSERT(gltfPrimimitive.Indices != -1, "No indicies");
			const GLTFAttributes& gltfAttributes = gltfPrimimitive.Attributes;
			MONK_ASSERT(gltfAttributes.Position != -1, "No POSITION in gltf mesh");
			MONK_ASSERT(gltfAttributes.Normal != -1, "No NORMAL in gltf mesh");

			const GLTFAccessor& gltfPositionAccessor = gltf.Accessros[gltfAttributes.Position];
			const GLTFAccessor& gltfNormalAccessor = gltf.Accessros[gltfAttributes.Normal];
			const GLTFAccessor& gltfIndicesAccessor = gltf.Accessros[gltfPrimimitive.Indices];

			MONK_ASSERT(gltfPositionAccessor.ComponentType == 5126); // GL_FLOAT
			MONK_ASSERT(gltfNormalAccessor.ComponentType == 5126); // GL_FLOAT
			MONK_ASSERT(gltfIndicesAccessor.ComponentType == 5125 || gltfIndicesAccessor.ComponentType == 5123); // GL_UNSIGNED_INT or GL_UNSINGED_SHORT
			MONK_ASSERT(gltfPositionAccessor.Type == GLTFAccessor::AccessorType::VEC3);
			MONK_ASSERT(gltfNormalAccessor.Type == GLTFAccessor::AccessorType::VEC3);
			MONK_ASSERT(gltfIndicesAccessor.Type == GLTFAccessor::AccessorType::SCALAR);

			const GLTFBufferView& gltfPositionBufferView = gltf.BufferViews[gltfPositionAccessor.BufferView];
			const GLTFBufferView& gltfNormalBufferView = gltf.BufferViews[gltfNormalAccessor.BufferView];
			const GLTFBufferView& gltfIndicesBufferView = gltf.BufferViews[gltfIndicesAccessor.BufferView];

			BufferLayout layout = {
				{ 0, BufferLayout::AttribType::Float3 },
			};
			uint8_t* data = buffers[gltfPositionBufferView.Buffer].Data;
	
			IndexBuffer::IndexType indexType = gltfIndicesAccessor.ComponentType == 5125 ? IndexBuffer::IndexType::UNSIGNED_INT : IndexBuffer::IndexType::UNSIGNED_SHORT;
			Shared<VertexBuffer> vertexBuffer = CreateShared<VertexBuffer>((float*)(data + gltfPositionBufferView.ByteOffset + gltfPositionAccessor.ByteOffset), gltfPositionBufferView.ByteLength - gltfPositionAccessor.ByteOffset, layout);
			Shared<IndexBuffer> indexBuffer = CreateShared<IndexBuffer>((void*)(data + gltfIndicesBufferView.ByteOffset + gltfIndicesAccessor.ByteOffset), gltfIndicesAccessor.Count, indexType, IndexBuffer::IndexUsage::STATIC);

			meshes.push_back(Mesh(vertexBuffer, indexBuffer, parentMatrix));
		}
		else
		{
			for (auto child : gltf.Nodes[node].Children)
			{
				auto childMeshes = ProcessNode(gltf, child, buffers, parentMatrix * gltf.Nodes[node].Matrix);
				meshes.insert(meshes.begin(), childMeshes.begin(), childMeshes.end());
			}
		}

		return meshes;
	}

	std::vector<FileData> ModelLoader::LoadBuffers(const std::vector<GLTFBuffer>& gltfBuffers)
	{
		std::vector<FileData> buffers;

		for (auto buffer : gltfBuffers)
		{
			FileData fileData = FileManager::ReadBytes(buffer.URI.string());
			MONK_ASSERT(fileData.Size == buffer.ByteLength, "Buffer size missmatch");

			buffers.push_back(fileData);
		}

		return buffers;
	}

	void ModelLoader::FreeBuffers(const std::vector<FileData> buffers)
	{
		for (auto fileData : buffers)
			fileData.Free();
	}

	math::mat4 ModelLoader::GetMatrixFromJSONList(const JSONList& list)
	{
		return math::Transpose(math::mat4(
			list[0 * 4 + 0]->GetNumber(),
			list[0 * 4 + 1]->GetNumber(),
			list[0 * 4 + 2]->GetNumber(),
			list[0 * 4 + 3]->GetNumber(),

			list[1 * 4 + 0]->GetNumber(),
			list[1 * 4 + 1]->GetNumber(),
			list[1 * 4 + 2]->GetNumber(),
			list[1 * 4 + 3]->GetNumber(),

			list[2 * 4 + 0]->GetNumber(),
			list[2 * 4 + 1]->GetNumber(),
			list[2 * 4 + 2]->GetNumber(),
			list[2 * 4 + 3]->GetNumber(),

			list[3 * 4 + 0]->GetNumber(),
			list[3 * 4 + 1]->GetNumber(),
			list[3 * 4 + 2]->GetNumber(),
			list[3 * 4 + 3]->GetNumber()
		));
	}

	Shared<Model> ModelLoader::LoadFromFile(const Filepath& filename)
	{
		GLTF gltf = LoadGLTF(filename);
		MONK_ASSERT(gltf.Scene != -1, "No default GLTF scene");
		Shared<Model> model = CreateShared<Model>();
		std::vector<Mesh> meshes;
		std::vector<FileData> buffers = LoadBuffers(gltf.Buffers);

		for (auto node : gltf.Scenes[gltf.Scene].Nodes)
		{
			auto nodeMeshes = ProcessNode(gltf, node, buffers);
			meshes.insert(meshes.end(), nodeMeshes.begin(), nodeMeshes.end());
		}

		model->m_Meshes = meshes;

		FreeBuffers(buffers);
		return model;
	}

	GLTFAccessor::AccessorType GLTFAccessor::ConvertToAccessorType(const std::string& type)
	{
		if (type == "SCALAR")
			return AccessorType::SCALAR;
		else if (type == "VEC2")
			return AccessorType::VEC2;
		else if (type == "VEC3")
			return AccessorType::VEC3;

		return AccessorType::NONE;
	}
}