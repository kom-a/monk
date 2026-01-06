#pragma once

#include <filesystem>
#include <optional>
#include <variant>

#include <MML/MML.h>

namespace nun
{
	typedef struct
	{
		std::string					Version;
		std::optional<std::string>	MinVersion;
		std::optional<std::string>	Generator;
		std::optional<std::string>	Copyright;
	} Asset_t;

	typedef struct
	{
		std::optional<std::string>	Name;
		std::vector<uint32_t>		Nodes;
	} Scene_t;

	typedef struct
	{
		std::optional<uint32_t>		Camera;
		std::vector<uint32_t>		Children;
		std::optional<uint32_t>		Skin;
		std::optional<mml::mat4>	Matrix;
		std::optional<uint32_t>		Mesh;
		std::optional<mml::vec4>	Rotation;
		std::optional<mml::vec3>	Scale;
		std::optional<mml::vec3>	Transltaion;
		std::vector<float>			Weights;
		std::optional<std::string>	Name;
	} Node_t;

	typedef struct
	{
		float XMag;
		float YMag;
		float ZFar;
		float ZNear;
	} CameraOrthographic;

	typedef struct
	{
		std::optional<float>	AspectRatio;
		float					YFov;
		std::optional<float>	ZFar;
		float					ZNear;
	} CameraPerspective;

	enum class CameraType
	{
		Perspective,
		Orthographic
	};

	typedef struct
	{
		std::optional<CameraOrthographic>	Orthographic;
		std::optional<CameraPerspective>	Perspective;
		CameraType							Type;
		std::optional<std::string>			Name;
	} Camera_t;

	typedef struct
	{
		uint32_t Position;
		uint32_t Normal;
		uint32_t Tangent;
		std::vector<uint32_t> TexCoords;
		std::vector<uint32_t> Colors;
		std::vector<uint32_t> Joints;
		std::vector<uint32_t> Weights;
	} MeshPrimitiveAttributes_t;

	enum MeshPrimitiveMode
	{
		POINTS			= 0,
		LINES			= 1,
		LINE_LOOP		= 2,
		LINE_STRIP		= 3,
		TRIANGLES		= 4,
		TRIANGLE_STRIP	= 5,
		TRIANGLE_FAN	= 6
	};

	typedef struct
	{
		MeshPrimitiveAttributes_t Attributes;
		std::optional<uint32_t> Indices;
		std::optional<uint32_t> Material;
		MeshPrimitiveMode Mode = MeshPrimitiveMode::TRIANGLES;
		// targets
	} MeshPrimitive_t;

	typedef struct
	{
		std::vector<MeshPrimitive_t> Primitives;
		std::vector<float> Weights;
		std::optional<std::string> Name;
	} Mesh_t;

	typedef struct
	{
		std::optional<std::string>	URI;
		uint32_t					ByteLength = 0;
		std::optional<std::string>	Name;
	} Buffer_t;

	enum BufferViewTarget
	{
		ARRAY_BUFFER = 34962,
		ELEMENT_ARRAY_BUFFER = 34963
	};

	typedef struct
	{
		uint32_t						Buffer;
		uint32_t						ByteOffset = 0;
		uint32_t						ByteLength;
		std::optional<uint32_t>			ByteStride;
		std::optional<BufferViewTarget> Target;
		std::optional<std::string>		Name;
	} BufferView_t;

	enum AccessorComponentType
	{
		BYTE = 5120,
		UNSIGNED_BYTE = 5121,
		SHORT = 5122,
		UNSIGNED_SHORT = 5123,
		UNSIGNED_INT = 5125,
		FLOAT = 5126
	};

	enum AccessorType
	{
		None,
		SCALAR,
		VEC2,
		VEC3,
		VEC4,
		MAT2,
		MAT3,
		MAT4
	};

	typedef struct
	{
		std::optional<uint32_t>				BufferView;
		uint32_t							ByteOffset = 0;
		AccessorComponentType				ComponentType;
		bool								Normalized = false;
		uint32_t							Count;
		AccessorType						Type;

		using MinMaxValue = std::variant<
			mml::vec2,
			mml::vec3,
			mml::vec4,
			// mml::mat2,
			// mml::mat3,
			mml::mat4>;

		std::optional<MinMaxValue>			Max;
		std::optional<MinMaxValue>			Min;
		std::optional<std::string>			Name;

	} Accessor_t;

	typedef struct
	{
		uint32_t Index;
		uint32_t TexCoord = 0;
	} TextureInfo_t;

	typedef struct
	{
		mml::vec4						BaseColorFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
		std::optional<TextureInfo_t>	BaseColorTexture;
		float							MetallicFactor = 1.0f;
		float							RoughnessFactor = 1.0f;
		std::optional<TextureInfo_t>	MetallicRoughnessTexture;
	} MaterialPBRMetallicRoughness_t;

	typedef struct
	{
		uint32_t Index;
		uint32_t TexCoord = 0;
		float Scale = 1.0f;
	} MaterialNormalTextureInfo_t;

	typedef struct
	{
		uint32_t Index;
		uint32_t TexCoord = 0;
		float Strength = 1.0f;
	} MaterialOcclusionTextureInfo_t;

	enum MaterialAlphaMode
	{
		OPAQUE,
		MASK,
		BLEND
	};

	typedef struct
	{
		std::optional<std::string>						Name;
		std::optional<MaterialPBRMetallicRoughness_t>	PBRMetallicRoughness;
		std::optional<MaterialNormalTextureInfo_t>		NormalTexture;
		std::optional<MaterialOcclusionTextureInfo_t>	OcclusionTexture;
		std::optional<TextureInfo_t>					EmissiveTexutre;
		mml::vec3										EmissiveFactor = { 0.0f, 0.0f, 0.0f };
		MaterialAlphaMode								AlphaMode = MaterialAlphaMode::OPAQUE;
		float											AlphaCutoff = 0.05f;
		bool											DoubleSided = false;
	} Material_t;

	typedef struct
	{

	} Texture_t;

	enum ImageMimeType
	{
		Image_JPEG,
		Image_PNG
	};

	typedef struct
	{
		std::optional<std::string> URI;
		std::optional<ImageMimeType> MimeType;
		std::optional<uint32_t> BufferView;
		std::optional<std::string> Name;
	} Image_t;

	typedef struct
	{

	} Sampler_t;

	typedef struct
	{

	} Skin_t;

	typedef struct
	{

	} Animation_t;

	class Model
	{
	public:
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

		std::filesystem::path GetModelFilepath() const;

		friend Model* LoadModelFromFile(const std::filesystem::path& path);

	private:
		std::filesystem::path m_ModelPath;
	};

	Model* LoadModelFromFile(const std::filesystem::path& path);
}