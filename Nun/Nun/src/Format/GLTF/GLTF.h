#pragma once

#include "../../../include/Nun/Nun.h"

#include <vector>
#include <optional>
#include <string>
#include <filesystem>
#include <variant>

#include <MML/MML.h>

namespace nun
{
	struct GLTF
	{
		std::vector<std::string>	ExtensionsUsed;
		std::vector<std::string>	ExtensionsRequired;

		std::vector<Accessor_t>		Accessors;
		std::vector<Animation_t>	Animations;
		Asset_t						Asset;
		std::vector<Buffer_t>		Buffers;
		std::vector<BufferView_t>	BufferViews;
		std::vector<Camera_t>		Cameras;
		std::vector<Image_t>		Images;
		std::vector<Material_t>		Materials;
		std::vector<Mesh_t>			Meshes;
		std::vector<Node_t>			Nodes;
		std::vector<Sampler_t>		Samplers;
		std::optional<uint32_t>		Scene;
		std::vector<Scene_t>		Scenes;
		std::vector<Skin_t>			Skins;
		std::vector<Texture_t>		Textures;
	};

	GLTF ParseGLTF(const std::filesystem::path& path);
}