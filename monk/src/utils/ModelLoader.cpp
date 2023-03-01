#include "ModelLoader.h"

#include "core/Log.h"
#include "core/Assert.h"
#include "utils/FileManager.h"

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

		const JSONNode json = *jsonPtr;
		GLTF gltf;
		gltf.Buffers = GetGLTFBuffers(json, filename);
		gltf.BufferViews = GetGLTFBufferViews(json);
		gltf.Accessros = GetGLTFAccessors(json);
		gltf.Meshes = GetGLTFMeshes(json);
		gltf.Nodes = GetGLTFNodes(json);

		return gltf;
	}

	std::vector<GLTFBuffer> ModelLoader::GetGLTFBuffers(const JSONNode& json, const Filepath& root)
	{
		std::vector<GLTFBuffer> gltfBuffers;
		auto buffersList = json["buffers"].GetList();

		for (auto buffer : buffersList)
		{
			GLTFBuffer gltfBuffer;
			gltfBuffer.ByteLength = (*buffer)["byteLength"].GetNumber();
			gltfBuffer.URI = root / (*buffer)["uri"].GetString();

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

	Shared<Model> ModelLoader::m_Model = nullptr;
	Filepath ModelLoader::m_Filepath = "No filepath";

	Shared<Model> ModelLoader::LoadFromFile(const Filepath& filename)
	{
		GLTF gltf = LoadGLTF(filename);
		

		return nullptr;
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