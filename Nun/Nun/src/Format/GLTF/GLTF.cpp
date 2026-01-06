#include "GLTF.h"

#include <Jsonetic/Jsonetic.h>

namespace nun
{
	template<typename T, typename U = T>
	static std::vector<T> ParseArray(const jsonc::Array& arr)
	{
		std::vector<T> result;

		for (const jsonc::Value& value : arr)
		{
			result.push_back(value.Get<U>());
		}

		return result;
	}

	static mml::mat4 ParseMatrix4(const jsonc::Array& arr)
	{
		std::vector<float> matrixArray = ParseArray<float, jsonc::Number>(arr);

		if (matrixArray.size() != 16)
			return mml::mat4(1.0f);

		return mml::mat4(
			matrixArray[0], matrixArray[4], matrixArray[ 8], matrixArray[12],
			matrixArray[1], matrixArray[5], matrixArray[ 9], matrixArray[13],
			matrixArray[2], matrixArray[6], matrixArray[10], matrixArray[14],
			matrixArray[3], matrixArray[7], matrixArray[11], matrixArray[15]
		);
	}

	static mml::vec3 ParseVec3(const jsonc::Array& arr)
	{
		std::vector<float> vecArray = ParseArray<float, jsonc::Number>(arr);

		if (vecArray.size() != 3)
			return mml::vec3(0.0f);

		return mml::vec3(vecArray[0], vecArray[1], vecArray[2]);
	}

	static mml::vec4 ParseVec4(const jsonc::Array& arr)
	{
		std::vector<float> vecArray = ParseArray<float, jsonc::Number>(arr);

		if (vecArray.size() != 4)
			return mml::vec4(0.0f);

		return mml::vec4(vecArray[0], vecArray[1], vecArray[2], vecArray[3]);
	}

	static AccessorType ParseAccessorType(const std::string& value)
	{
		if (value == "SCALAR")
			return AccessorType::SCALAR;
		else if (value == "VEC2")
			return AccessorType::VEC2;
		else if (value == "VEC3")
			return AccessorType::VEC3;
		else if (value == "VEC4")
			return AccessorType::VEC4;
		else if (value == "MAT2")
			return AccessorType::MAT2;
		else if (value == "MAT3")
			return AccessorType::MAT3;
		else if (value == "MAT4")
			return AccessorType::MAT4;
		
		return AccessorType::None;
	}

	static Accessor_t::MinMaxValue ParseMinMaxValue(const jsonc::Array& minmax, AccessorType type)
	{
		if (type == AccessorType::VEC2 && minmax.size() == 2)
			return mml::vec2(
				minmax[0].Get<jsonc::Number>(), 
				minmax[1].Get<jsonc::Number>()
			);
		else if(type == AccessorType::VEC3 && minmax.size() == 3)
			return mml::vec3(
				minmax[0].Get<jsonc::Number>(), 
				minmax[1].Get<jsonc::Number>(), 
				minmax[2].Get<jsonc::Number>()
			);
		else if (type == AccessorType::VEC4 && minmax.size() == 4)
			return mml::vec4(
				minmax[0].Get<jsonc::Number>(),
				minmax[1].Get<jsonc::Number>(),
				minmax[2].Get<jsonc::Number>(),
				minmax[3].Get<jsonc::Number>()
			);
		else if (type == AccessorType::MAT4 && minmax.size() == 4 * 4)
			return mml::mat4(
				minmax[0].Get<jsonc::Number>(),
				minmax[1].Get<jsonc::Number>(),
				minmax[2].Get<jsonc::Number>(),
				minmax[3].Get<jsonc::Number>(),

				minmax[4].Get<jsonc::Number>(),
				minmax[5].Get<jsonc::Number>(),
				minmax[6].Get<jsonc::Number>(),
				minmax[7].Get<jsonc::Number>(),

				minmax[8].Get<jsonc::Number>(),
				minmax[9].Get<jsonc::Number>(),
				minmax[10].Get<jsonc::Number>(),
				minmax[11].Get<jsonc::Number>(),

				minmax[12].Get<jsonc::Number>(),
				minmax[13].Get<jsonc::Number>(),
				minmax[14].Get<jsonc::Number>(),
				minmax[15].Get<jsonc::Number>()
			);

		return {};
	}

	std::vector<Accessor_t> ParseAccessors(const jsonc::Array& accessors)
	{
		std::vector<Accessor_t> result;

		for (const jsonc::Value& value : accessors)
		{
			const jsonc::Object json = value.Get<jsonc::Object>();
			Accessor_t accessor;

			if (json.Has("bufferView"))
				accessor.BufferView = json["bufferView"].Get<jsonc::Number>();
			if (json.Has("byteOffset"))
				accessor.ByteOffset = json["byteOffset"].Get<jsonc::Number>();
			if (json.Has("componentType"))
				accessor.ComponentType = (AccessorComponentType)json["componentType"].Get<jsonc::Number>();
			if (json.Has("normalized"))
				accessor.Normalized = json["normalized"].Get<bool>();
			if (json.Has("count"))
				accessor.Count = json["count"].Get<jsonc::Number>();
			if (json.Has("type"))
				accessor.Type = ParseAccessorType(json["type"].Get<std::string>());
			if (json.Has("max"))
				accessor.Max = ParseMinMaxValue(json["max"].Get<jsonc::Array>(), accessor.Type);
			if (json.Has("min"))
				accessor.Min = ParseMinMaxValue(json["min"].Get<jsonc::Array>(), accessor.Type);
			if (json.Has("name"))
				accessor.Name = json["name"].Get<std::string>();

			result.push_back(accessor);
		}

		return result;
	}

	std::vector<Animation_t> ParseAnimations(const jsonc::Array& animations)
	{
		return {};
	}

	Asset_t ParseAsset(const jsonc::Object& asset)
	{
		Asset_t result;

		if (asset.Has("version"))
			result.Version = asset["version"].Get<std::string>();
		if(asset.Has("minVersion"))
			result.MinVersion = asset["minVersion"].Get<std::string>();
		if(asset.Has("generator"))
			result.Generator = asset["generator"].Get<std::string>();
		if (asset.Has("copyright"))
			result.Copyright = asset["copyright"].Get<std::string>();

		return result;
	}

	std::vector<Buffer_t> ParseBuffers(const jsonc::Array& buffers)
	{
		std::vector<Buffer_t> result;

		for (const jsonc::Value& value : buffers)
		{
			const jsonc::Object json = value.Get<jsonc::Object>();
			Buffer_t buffer;

			if (json.Has("uri"))
				buffer.URI = json["uri"].Get<std::string>();
			if (json.Has("byteLength"))
				buffer.ByteLength = json["byteLength"].Get<jsonc::Number>();
			if (json.Has("name"))
				buffer.Name = json["name"].Get<std::string>();

			result.push_back(buffer);
		}

		return result;
	}

	std::vector<BufferView_t> ParseBufferViews(const jsonc::Array& bufferViews)
	{
		std::vector<BufferView_t> result;

		for (const jsonc::Value& value : bufferViews)
		{
			const jsonc::Object json = value.Get<jsonc::Object>();
			BufferView_t bufferView;

			if (json.Has("buffer"))
				bufferView.Buffer = json["buffer"].Get<jsonc::Number>();
			if (json.Has("byteOffset"))
				bufferView.ByteOffset = json["byteOffset"].Get<jsonc::Number>();
			if (json.Has("byteLength"))
				bufferView.ByteLength = json["byteLength"].Get<jsonc::Number>();
			if (json.Has("byteStride"))
				bufferView.ByteStride = json["byteStride"].Get<jsonc::Number>();
			if (json.Has("target"))
				bufferView.Target = (BufferViewTarget)json["target"].Get<jsonc::Number>();
			if (json.Has("name"))
				bufferView.Name= json["name"].Get<std::string>();

			result.push_back(bufferView);
		}

		return result;
	}

	static CameraOrthographic ParseCameraOrthographic(const jsonc::Object& orthographic)
	{
		CameraOrthographic result;

		if (orthographic.Has("xmag"))
			result.XMag = orthographic["xmag"].Get<jsonc::Number>();
		if (orthographic.Has("ymag"))
			result.YMag = orthographic["ymag"].Get<jsonc::Number>();
		if (orthographic.Has("zfar"))
			result.ZFar = orthographic["zfar"].Get<jsonc::Number>();
		if (orthographic.Has("znear"))
			result.ZNear = orthographic["znear"].Get<jsonc::Number>();

		return result;
	}

	static CameraPerspective ParseCameraPerspective(const jsonc::Object& perspective)
	{
		CameraPerspective result;

		if (perspective.Has("aspectRatio"))
			result.AspectRatio = perspective["aspectRatio"].Get<jsonc::Number>();
		if (perspective.Has("yfov"))
			result.YFov = perspective["yfov"].Get<jsonc::Number>();
		if (perspective.Has("zfar"))
			result.ZFar = perspective["zfar"].Get<jsonc::Number>();
		if (perspective.Has("znear"))
			result.ZNear = perspective["znear"].Get<jsonc::Number>();

		return result;
	}

	static CameraType ParseCameraType(const std::string& type)
	{
		if (type == "orthographic")
			return CameraType::Orthographic;
		else if (type == "perspective")
			return CameraType::Perspective;

		return {};
	}

	std::vector<Camera_t> ParseCameras(const jsonc::Array& cameras)
	{
		std::vector<Camera_t> result;

		for (const jsonc::Value& value : cameras)
		{
			const jsonc::Object json = value.Get<jsonc::Object>();
			Camera_t camera;

			if (json.Has("orthographic"))
				camera.Orthographic = ParseCameraOrthographic(json["orthographic"].Get<jsonc::Object>());
			if (json.Has("perspective"))
				camera.Perspective = ParseCameraPerspective(json["perspective"].Get<jsonc::Object>());
			if (json.Has("type"))
				camera.Type = ParseCameraType(json["type"].Get<std::string>());
			if (json.Has("name"))
				camera.Name = json["name"].Get<std::string>();

			result.push_back(camera);
		}

		return result;
	}

	std::vector<Image_t> ParseImages(const jsonc::Array& images)
	{
		return {};
	}

	std::vector<Material_t>	ParseMaterials(const jsonc::Array& materials)
	{
		return {};
	}

	static MeshPrimitiveAttributes_t ParseAttributes(const jsonc::Object& attributes)
	{
		MeshPrimitiveAttributes_t result;

		if (attributes.Has("POSITION"))
			result.Position = attributes["POSITION"].Get<jsonc::Number>();
		if (attributes.Has("NORMAL"))
			result.Normal = attributes["NORMAL"].Get<jsonc::Number>();
		if (attributes.Has("TANGENT"))
			result.Tangent = attributes["TANGENT"].Get<jsonc::Number>();

		const std::string texCoordPrefix	= "TEXCOORD_";
		const std::string colorsPrefix		= "COLOR_";
		const std::string jointsPrefix		= "JOINTS_";
		const std::string weightsPrefix		= "WEIGHTS_";

		size_t texCoordIndex	= 0;
		size_t colorsIndex		= 0;
		size_t jointsIndex		= 0;
		size_t weightsIndex		= 0;

		while (attributes.Has(texCoordPrefix + std::to_string(texCoordIndex)))
			result.TexCoords.push_back(attributes[texCoordPrefix + std::to_string(texCoordIndex++)].Get<jsonc::Number>());

		while (attributes.Has(colorsPrefix + std::to_string(colorsIndex)))
			result.Colors.push_back(attributes[colorsPrefix + std::to_string(colorsIndex++)].Get<jsonc::Number>());

		while (attributes.Has(jointsPrefix + std::to_string(jointsIndex)))
			result.TexCoords.push_back(attributes[jointsPrefix + std::to_string(jointsIndex++)].Get<jsonc::Number>());

		while (attributes.Has(weightsPrefix + std::to_string(weightsIndex)))
			result.TexCoords.push_back(attributes[weightsPrefix + std::to_string(weightsIndex++)].Get<jsonc::Number>());

		return result;
	}

	static std::vector<MeshPrimitive_t> ParseMeshPrimitives(const jsonc::Array& primitives)
	{
		std::vector<MeshPrimitive_t> result;

		for (const jsonc::Value& value : primitives)
		{
			const jsonc::Object json = value.Get<jsonc::Object>();
			MeshPrimitive_t primitive;

			if (json.Has("attributes"))
				primitive.Attributes = ParseAttributes(json["attributes"].Get<jsonc::Object>());
			if (json.Has("indices"))
				primitive.Indices = json["indices"].Get<jsonc::Number>();
			if (json.Has("material"))
				primitive.Material = json["material"].Get<jsonc::Number>();
			if (json.Has("mode"))
				primitive.Mode = (MeshPrimitiveMode)json["mode"].Get<jsonc::Number>();

			result.push_back(primitive);
		}

		return result;
	}

	std::vector<Mesh_t>	ParseMeshes(const jsonc::Array& meshes)
	{
		std::vector<Mesh_t> result;

		for (const jsonc::Value& value : meshes)
		{
			const jsonc::Object json = value.Get<jsonc::Object>();
			Mesh_t mesh;

			if (json.Has("primitives"))
				mesh.Primitives = ParseMeshPrimitives(json["primitives"].Get<jsonc::Array>());
			if (json.Has("weights"))
				mesh.Weights = ParseArray<float, jsonc::Number>(json["weights"].Get<jsonc::Array>());
			if (json.Has("name"))
				mesh.Name = json["name"].Get<std::string>();
			
			result.push_back(mesh);
		}

		return result;
	}

	std::vector<Node_t>	ParseNodes(const jsonc::Array& nodes)
	{
		std::vector<Node_t> result;

		for (const jsonc::Value& value : nodes)
		{
			const jsonc::Object json = value.Get<jsonc::Object>();
			Node_t node;

			if (json.Has("camera"))
				node.Camera = json["camera"].Get<jsonc::Number>();
			if (json.Has("children"))
				node.Children = ParseArray<uint32_t, jsonc::Number>(json["children"].Get<jsonc::Array>());
			if (json.Has("skin"))
				node.Skin = json["skin"].Get<jsonc::Number>();
			if (json.Has("matrix"))
				node.Matrix = ParseMatrix4(json["matrix"].Get<jsonc::Array>());			
			if (json.Has("mesh"))
				node.Mesh = json["mesh"].Get<jsonc::Number>();
			if (json.Has("rotation"))
				node.Rotation = ParseVec4(json["rotation"].Get<jsonc::Array>());
			if (json.Has("scale"))
				node.Scale = ParseVec3(json["scale"].Get<jsonc::Array>());
			if (json.Has("translation"))
				node.Transltaion = ParseVec3(json["translation"].Get<jsonc::Array>());
			if (json.Has("weights"))
				node.Weights = ParseArray<float, jsonc::Number>(json["weights"].Get<jsonc::Array>());
			if (json.Has("name"))
				node.Name = json["name"].Get<std::string>();


			result.push_back(node);
		}

		return result;
	}

	std::vector<Sampler_t> ParseSamplers(const jsonc::Array& samples)
	{
		return {};
	}

	std::vector<Scene_t> ParseScenes(const jsonc::Array& scenes)
	{
		std::vector<Scene_t> result;

		for (const jsonc::Value& value : scenes)
		{
			const jsonc::Object json = value.Get<jsonc::Object>();
			Scene_t scene;

			if (json.Has("name"))
				scene.Name = json["name"].Get<std::string>();
			if (json.Has("nodes"))
				scene.Nodes = ParseArray<uint32_t, jsonc::Number>(json["nodes"].Get<jsonc::Array>());


			result.push_back(scene);
		}

		return result;
	}

	std::vector<Skin_t>	ParseSkins(const jsonc::Array& skins)
	{
		return {};
	}

	std::vector<Texture_t> ParseTextures(const jsonc::Array& textures)
	{
		return {};
	}

	GLTF ParseGLTF(const std::filesystem::path& path)
	{
		GLTF gltf;

		jsonc::Object json = jsonc::ParseFile(path.string());

		if (json.Has("accessors"))
			gltf.Accessors = ParseAccessors(json["accessors"].Get<jsonc::Array>());
		if (json.Has("animations"))
			gltf.Animations = ParseAnimations(json["animations"].Get<jsonc::Array>());
		if (json.Has("asset"))
			gltf.Asset = ParseAsset(json["asset"].Get<jsonc::Object>());
		if (json.Has("buffers"))
			gltf.Buffers = ParseBuffers(json["buffers"].Get<jsonc::Array>());
		if (json.Has("bufferViews"))
			gltf.BufferViews = ParseBufferViews(json["bufferViews"].Get<jsonc::Array>());
		if (json.Has("cameras"))
			gltf.Cameras = ParseCameras(json["cameras"].Get<jsonc::Array>());
		if (json.Has("images"))
			gltf.Images = ParseImages(json["images"].Get<jsonc::Array>());
		if (json.Has("materials"))
			gltf.Materials = ParseMaterials(json["materials"].Get<jsonc::Array>());
		if (json.Has("meshes"))
			gltf.Meshes = ParseMeshes(json["meshes"].Get<jsonc::Array>());
		if (json.Has("nodes"))
			gltf.Nodes = ParseNodes(json["nodes"].Get<jsonc::Array>());
		if (json.Has("samplers"))
			gltf.Samplers = ParseSamplers(json["samplers"].Get<jsonc::Array>());
		if (json.Has("scene"))
			gltf.Scene = json["scene"].Get<jsonc::Number>();
		if (json.Has("scenes"))
			gltf.Scenes = ParseScenes(json["scenes"].Get<jsonc::Array>());
		if (json.Has("skins"))
			gltf.Skins = ParseSkins(json["skins"].Get<jsonc::Array>());
		if (json.Has("textures"))
			gltf.Textures = ParseTextures(json["textures"].Get<jsonc::Array>());

		return gltf;
	}
}