#include "../include/Nun/Nun.h"

#include "Format/GLTF/GLTF.h"

namespace nun
{
	enum class ModelType
	{
		None = 0,
		GLTF
	};

	static ModelType GetModelType(const std::filesystem::path& path)
	{
		if (path.extension() == ".gltf")
			return ModelType::GLTF;

		return ModelType::None;
	}

	static Model* LoadModelGLTF(const std::filesystem::path& path)
	{
		GLTF gltf = ParseGLTF(path);
		Model* model = new Model();
		
		model->Accessors	= gltf.Accessors;
		model->Animations	= gltf.Animations;
		model->Asset		= gltf.Asset;
		model->Buffers		= gltf.Buffers;
		model->BufferViews	= gltf.BufferViews;
		model->Cameras		= gltf.Cameras;
		model->Images		= gltf.Images;
		model->Materials	= gltf.Materials;
		model->Meshes		= gltf.Meshes;
		model->Nodes		= gltf.Nodes;
		model->Samplers		= gltf.Samplers;
		model->Scene		= gltf.Scene;
		model->Scenes		= gltf.Scenes;
		model->Skins		= gltf.Skins;
		model->Textures		= gltf.Textures;
		
		return model;
	}

	nun::Model* LoadModelFromFile(const std::filesystem::path& path)
	{
		Model* model = nullptr;

		if (GetModelType(path) == ModelType::GLTF)
		{
			model = LoadModelGLTF(path);
			model->m_ModelPath = path.parent_path();
		}

		return model;
	}

	std::filesystem::path Model::GetModelFilepath() const
	{
		return m_ModelPath;
	}

}