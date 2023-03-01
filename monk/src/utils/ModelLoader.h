#pragma once

#include <string>
#include <filesystem>

#include "core/Memory.h"
#include "graphics/Model.h"
#include "utils/JSON.h"

namespace monk
{
	struct GLTFAccessor
	{
		uint32_t BufferView = -1;
		uint32_t ByteOffset = -1;
		uint32_t ComponentType = -1;
		uint32_t Count = -1;
		std::vector<uint32_t> Max;
		std::vector<uint32_t> Min;
		enum class AccessorType
		{
			SCALAR, VEC2, VEC3, NONE
		} Type = AccessorType::NONE;

		static AccessorType ConvertToAccessorType(const std::string& type);
	};

	struct GLTFBufferView
	{
		uint32_t Buffer = -1;
		uint32_t ByteLength = -1;
		uint32_t ByteOffset = -1;
		uint32_t ByteStride = -1;
		std::string Name = "None";
		uint32_t Target = -1;
	};

	struct GLTFBuffer
	{
		uint32_t ByteLength = -1;
		Filepath URI;
	};

	struct GLTFAttributes
	{
		uint32_t Position = -1;
		uint32_t Normal = -1;
		uint32_t TexCoord_0 = -1;
	};

	struct GLTFPrimitive
	{
		GLTFAttributes Attributes;
		uint32_t Indices = -1;
		uint32_t Material = -1;
		uint32_t Mode = -1;
	};

	struct GLTFMesh
	{
		std::string Name;
		std::vector<GLTFPrimitive> Primitives;
	};

	struct GLTFNode
	{
		uint32_t Camera = -1;
		std::vector<uint32_t> Children;
		math::mat4 Matrix;
		uint32_t Mesh;
		math::vec3 Translation;
		math::vec4 Rotation;
		math::vec3 Scale;
		std::string Name;

		bool UseMatrix;
	};

	struct GLTF
	{
		std::vector<GLTFBuffer> Buffers;
		std::vector<GLTFAccessor> Accessros;
		std::vector<GLTFBufferView> BufferViews;
		std::vector<GLTFMesh> Meshes;
		std::vector<GLTFNode> Nodes;
	};

	class ModelLoader
	{
	public:
		ModelLoader() = delete;
		~ModelLoader() = delete;

	public:
		static Shared<Model> LoadFromFile(const Filepath& filename);

	private:
		static GLTF LoadGLTF(const Filepath& filename);
		static std::vector<GLTFBuffer> GetGLTFBuffers(const JSONNode& json, const Filepath& root);
		static std::vector<GLTFBufferView> GetGLTFBufferViews(const JSONNode& json);
		static std::vector<GLTFAccessor> GetGLTFAccessors(const JSONNode& json);
		static std::vector<GLTFMesh> GetGLTFMeshes(const JSONNode& json);
		static std::vector<GLTFNode> GetGLTFNodes(const JSONNode& json);
		static std::vector<GLTFPrimitive> GetGLTFPrimitives(const JSONList& list);
		static GLTFAttributes GetGLTFAttributes(const JSONNode& json);

		template <typename T>
		static std::vector<T> GetNumbersFromJSONList(const JSONList& list);

		static math::mat4 GetMatrixFromJSONList(const JSONList& list);
	};

	template <typename T>
	static std::vector<T> ModelLoader::GetNumbersFromJSONList(const JSONList& list)
	{
		std::vector<T> numbers;

		for (auto n : list)
		{
			numbers.push_back(n->GetNumber());
		}

		return numbers;
	}

}