#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>

#include <Nun/Nun.h>

#include <MOGL/MOGL.h>
#include <MWL/MWL.h>

const char* g_VertexShaderSource = R"(
	#version 460 core

	layout (location = 0) in vec3 a_Position;
	layout (location = 1) in vec3 a_Normal;

	uniform mat4 u_Model;
	uniform mat4 u_View;
	uniform mat4 u_Projection;

	out vec3 v_Position;
	out vec3 v_Normal;

	void main()
	{
		gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0f);

		v_Position = a_Position;
		v_Normal = a_Normal;
	}
)";

const char* g_FragmentShaderSource = R"(
	#version 460 core

	out vec4 o_FragColor;

	in vec3 v_Position;
	in vec3 v_Normal;
	
	void main()
	{
		vec3 lightPos = vec3(15000.0f, -7000.0f, -10000.0f);
		vec3 lightDir = normalize(lightPos - v_Position);

		vec3 color = vec3(1.0f, 1.0f, 1.0f);

		vec3 ambient = color * 0.25f;
		vec3 diffuse = color * max(0.0f, dot(v_Normal, lightDir));

		o_FragColor = vec4(ambient + diffuse, 1.0f);
	}
)";

int AccessorTypeToOpenGLSize(nun::AccessorType type)
{
	switch (type)
	{
	case nun::AccessorType::None: return 0;
	case nun::AccessorType::SCALAR: return 1;
	case nun::AccessorType::VEC2: return 2;
	case nun::AccessorType::VEC3: return 3;
	case nun::AccessorType::VEC4: return 4;
	case nun::AccessorType::MAT2: return 2;
	case nun::AccessorType::MAT3: return 3;
	case nun::AccessorType::MAT4: return 4;
	}

	return 0;
}

mwl::Window* g_Window = nullptr;

mml::vec2 g_MousePosition;
mml::vec2 g_LastMousePosition;
bool g_MouseButtonDown = false;

struct Camera
{
	mml::vec3 Position;
	mml::vec3 LookDirection;
	mml::mat4 ViewMatrix;

	mml::vec3 m_RightDirection;
	mml::vec3 m_UpDirection;

	float Speed = 20.0f;
	float m_Yaw = 90, m_Pitch = 0;
	float m_Sensitivity = 0.05f;

	Camera(const mml::vec3& position, const mml::vec3& lookDirection)
		: Position(position), LookDirection(mml::Normalize(lookDirection)), ViewMatrix(mml::LookAt(Position, Position + LookDirection))
	{ }

	void Update(float deltaTime)
	{
		float speed = Speed * deltaTime;

		if (g_Window->GetKeyCodeState(mwl::KeyCode::W) == mwl::KeyCodeState::Down)
			Position += LookDirection * speed;
		if (g_Window->GetKeyCodeState(mwl::KeyCode::S) == mwl::KeyCodeState::Down)
			Position -= LookDirection * speed;
		if (g_Window->GetKeyCodeState(mwl::KeyCode::D) == mwl::KeyCodeState::Down)
			Position -= mml::Normalize(mml::Cross(LookDirection, mml::vec3(0.0f, 1.0f, 0.0f))) * speed;
		if (g_Window->GetKeyCodeState(mwl::KeyCode::A) == mwl::KeyCodeState::Down)
			Position += mml::Normalize(mml::Cross(LookDirection, mml::vec3(0.0f, 1.0f, 0.0f))) * speed;
		if (g_Window->GetKeyCodeState(mwl::KeyCode::Space) == mwl::KeyCodeState::Down)
			Position += mml::vec3(0.0f, 1.0f, 0.0f) * speed;
		if (g_Window->GetKeyCodeState(mwl::KeyCode::LeftControl) == mwl::KeyCodeState::Down)
			Position -= mml::vec3(0.0f, 1.0f, 0.0f) * speed;

		ViewMatrix = mml::LookAt(Position, Position + LookDirection);

		if (!g_MouseButtonDown)
			return;

		mml::vec2 mouseOffset = g_LastMousePosition - g_MousePosition;
		m_Yaw += mouseOffset.x * m_Sensitivity;
		m_Pitch += mouseOffset.y * m_Sensitivity;
		m_Pitch = mml::Clamp(m_Pitch, -89.0f, 89.0f);

		LookDirection.x = std::cos(mml::ToRadians(m_Yaw)) * std::cos(mml::ToRadians(m_Pitch));
		LookDirection.y = std::sin(mml::ToRadians(m_Pitch));
		LookDirection.z = std::sin(mml::ToRadians(m_Yaw)) * std::cos(mml::ToRadians(m_Pitch));
		LookDirection.normalize();

		m_RightDirection = mml::Normalize(mml::Cross(m_UpDirection, LookDirection));

		ViewMatrix = mml::LookAt(Position, Position + LookDirection);
	}
};

void OnMouseScroll(mwl::MouseScrollEvent& e)
{
}

void OnMouseButtonDown(mwl::MouseButtonDownEvent& e)
{
	if (e.Button == mwl::MouseButton::Right)
		g_MouseButtonDown = true;
}

void OnMouseButtonUp(mwl::MouseButtonUpEvent& e)
{
	if (e.Button == mwl::MouseButton::Right)
		g_MouseButtonDown = false;
}

void OnMouseMove(mwl::MouseMovedEvent& e)
{
	g_MousePosition.x = e.X;
	g_MousePosition.y = e.Y;
}

void OnKeyDown(mwl::KeyDownEvent& e)
{
}

void OnKeyUp(mwl::KeyUpEvent& e)
{
}

void CheckShaderError(uint32_t object, uint32_t status)
{
	switch (status)
	{
	case GL_COMPILE_STATUS:
	{
		int success;
		glGetShaderiv(object, status, &success);
		if (!success)
		{
			int length;
			glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = (char*)alloca(length);
			glGetShaderInfoLog(object, length, &length, infoLog);
			std::cout << infoLog << std::endl;
		}
	} break;
	case GL_LINK_STATUS:
	{
		int success;
		glGetProgramiv(object, status, &success);
		if (!success)
		{
			int length;
			glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = (char*)alloca(length);
			glGetProgramInfoLog(object, length, &length, infoLog);
			std::cout << infoLog << std::endl;
		}
	} break;
	}
}

uint32_t CompileShader(std::string_view shaderSource, uint32_t type)
{
	uint32_t shader = glCreateShader(type);

	const char* src = shaderSource.data();
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS);

	return shader;
}

uint32_t CreateShader(const char* vertexSource, const char* fragmentSource)
{
	uint32_t vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
	uint32_t fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	uint32_t shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);

	CheckShaderError(shader, GL_LINK_STATUS);

	return shader;
}

struct Mesh
{
	GLuint VAO;
	GLuint VBO_Positions;
	GLuint VBO_Normals;
	GLuint EBO;
	size_t indexCount;
	uint32_t indexType;
	uint32_t drawType;

	mml::mat4 modelMatrix = mml::mat4(1.0f);
};

std::vector<std::vector<uint8_t>> GetBuffers(const std::vector<nun::Buffer_t>& buffers, const std::filesystem::path& rootPath)
{
	std::vector<std::vector<uint8_t>> result;
	for (size_t i = 0; i < buffers.size(); i++)
	{
		std::filesystem::path bufferPath = rootPath / *buffers[i].URI;

		std::ifstream file(bufferPath, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> buffer(size);

		if (!file.read((char*)buffer.data(), size))
		{
			std::cout << "Failed to read buffer " << std::endl;
		}

		file.close();

		result.push_back(buffer);
	}

	return result;
}

struct Model
{
	Model(const nun::Model& model)
	{
		Buffers = GetBuffers(model.Buffers, model.GetModelFilepath());
		uint32_t sceneIndex = model.Scene.value_or(0);
		const nun::Scene_t& scene = model.Scenes[sceneIndex];

		for (uint32_t nodeIndex : scene.Nodes)
		{
			ProcessNode(model, nodeIndex);
		}

		std::vector<mml::mat4> modelMatrixStack;

		Buffers.clear();
	}

	mml::mat4 quatToMat4(const mml::vec4& q)
	{
		float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
		float xx = q.x * x2, yy = q.y * y2, zz = q.z * z2;
		float xy = q.x * y2, xz = q.x * z2, yz = q.y * z2;
		float wx = q.w * x2, wy = q.w * y2, wz = q.w * z2;

		return {
			1 - (yy + zz),  xy + wz,      xz - wy,     0,
			xy - wz,      1 - (xx + zz),  yz + wx,     0,
			xz + wy,      yz - wx,      1 - (xx + yy), 0,
			0,            0,            0,           1
		};
	}

	void ProcessNode(const nun::Model& model, uint32_t nodeIndex)
	{
		const nun::Node_t& node = model.Nodes[nodeIndex];

		if (node.Matrix)
			ModelMatrixStack.push_back(*node.Matrix);
		else if (node.Transltaion || node.Rotation || node.Scale)
		{
			mml::mat4 T(1.0f);
			mml::mat4 R(1.0f);
			mml::mat4 S(1.0f);

			if (node.Transltaion)
			{
				const mml::vec3& t = *node.Transltaion;

				T = {
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					t.x, t.y, t.z, 1
				};
			}

			if (node.Rotation)
			{
				const mml::vec4 q = *node.Rotation;

				//R = quatToMat4(q);
			}

			if (node.Scale)
			{
				const mml::vec3& s = *node.Scale;

				S = {
					s.x, 0,   0,   0,
					0,   s.y, 0,   0,
					0,   0,   s.z, 0,
					0,   0,   0,   1
				};
			}

			T = mml::Transpose(T);
			R = mml::Transpose(R);
			S = mml::Transpose(S);

			ModelMatrixStack.push_back(mml::mat4(1.0f));
		}

		if (node.Mesh)
			ProcessMesh(model, *node.Mesh);

		for (uint32_t childIndex : node.Children)
			ProcessNode(model, childIndex);

		if (node.Matrix)
			ModelMatrixStack.pop_back();
	}

	mml::mat4 CalculateModelMatrix()
	{
		mml::mat4 result(1.0f);

		for (const mml::mat4& matrix : ModelMatrixStack)
		{
			result *= matrix;
		}

		return result;
	}

	void ProcessMesh(const nun::Model& model, uint32_t meshIndex)
	{
		const nun::Mesh_t& mesh = model.Meshes[meshIndex];

		for (const nun::MeshPrimitive_t& primitive : mesh.Primitives)
		{
			Mesh m;
			glGenVertexArrays(1, &m.VAO);
			glGenBuffers(1, &m.VBO_Positions);
			glGenBuffers(1, &m.VBO_Normals);
			glGenBuffers(1, &m.EBO);

			glBindVertexArray(m.VAO);

			// --- Позиции ---
			const auto& posAcc = model.Accessors[primitive.Attributes.Position];
			const auto& posView = model.BufferViews[*posAcc.BufferView];
			const auto& posBuf = Buffers[posView.Buffer];

			// Абсолютное смещение в исходном буфере:
			const size_t posOffset = posView.ByteOffset + posAcc.ByteOffset;
			const size_t posCount = posAcc.Count * AccessorTypeToOpenGLSize(posAcc.Type);
			const size_t posSize = posCount * sizeof(float);
			const void* posData = posBuf.data() + posOffset;

			glBindBuffer(GL_ARRAY_BUFFER, m.VBO_Positions);
			glBufferData(GL_ARRAY_BUFFER, posSize, posData, GL_STATIC_DRAW);

			// stride: либо задан interleaved, либо tightly-packed (3 компоненты)
			GLsizei posStride = posView.ByteStride.value_or(sizeof(float) * AccessorTypeToOpenGLSize(posAcc.Type));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(
				0,
				AccessorTypeToOpenGLSize(posAcc.Type),   // число компонентов (vec3 → 3)
				posAcc.ComponentType,                    // GL_FLOAT = 5126
				GL_FALSE,
				posStride,
				(void*)0                                 // смещение внутри VBO уже учтено при загрузке
			);

			// --- Нормали ---
			const auto& normAcc = model.Accessors[primitive.Attributes.Normal];
			const auto& normView = model.BufferViews[*normAcc.BufferView];
			const auto& normBuf = Buffers[normView.Buffer];
			const size_t normOffset = normView.ByteOffset + normAcc.ByteOffset;
			const size_t normCount = normAcc.Count * AccessorTypeToOpenGLSize(normAcc.Type);
			const size_t normSize = normCount * sizeof(float);
			const void* normData = normBuf.data() + normOffset;

			glBindBuffer(GL_ARRAY_BUFFER, m.VBO_Normals);
			glBufferData(GL_ARRAY_BUFFER, normSize, normData, GL_STATIC_DRAW);

			GLsizei normStride = normView.ByteStride.value_or(sizeof(float) * AccessorTypeToOpenGLSize(normAcc.Type));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1,
				AccessorTypeToOpenGLSize(normAcc.Type),
				normAcc.ComponentType,
				GL_FALSE,
				normStride,
				(void*)0
			);

			// --- Индексы ---
			const auto& idxAcc = model.Accessors[*primitive.Indices];
			const auto& idxView = model.BufferViews[*idxAcc.BufferView];
			const auto& idxBuf = Buffers[idxView.Buffer];

			// аналогично позиции и нормалям:
			const size_t idxOffset = idxView.ByteOffset + idxAcc.ByteOffset;
			const size_t idxCount = idxAcc.Count;
			const size_t idxSize = idxCount * (idxAcc.ComponentType == nun::AccessorComponentType::UNSIGNED_SHORT
				? sizeof(uint16_t)
				: sizeof(uint32_t));
			const void* idxData = idxBuf.data() + idxOffset;

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize, idxData, GL_STATIC_DRAW);

			// Сохраняем параметры для отрисовки
			m.indexCount = static_cast<GLsizei>(idxAcc.Count);
			m.indexType = (idxAcc.ComponentType == nun::AccessorComponentType::UNSIGNED_SHORT
				? GL_UNSIGNED_SHORT
				: GL_UNSIGNED_INT);
			m.drawType = primitive.Mode;             // обычно GL_TRIANGLES
			m.modelMatrix = CalculateModelMatrix();

			Meshes.push_back(m);
			glBindVertexArray(0);
		}
	}

	std::vector<std::vector<uint8_t>> Buffers;
	std::vector<mml::mat4> ModelMatrixStack;
	std::vector<Mesh> Meshes;
};

int main()
{
	mwl::WindowProps props;
	props.Width = 1600 / 2;
	props.Height = 900 / 2;
	props.OpenGLContextVersion = mwl::OpenGLVersion::OPENGL_4_6;
	props.Title = L"Nun Sandbox";
	props.VSync = true;

	g_Window = mwl::Create(props);
	g_Window->SetMouseScrollCalback(OnMouseScroll);
	g_Window->SetMouseButtonDownCallback(OnMouseButtonDown);
	g_Window->SetMouseButtonUpCallback(OnMouseButtonUp);
	g_Window->SetMouseMovedCallback(OnMouseMove);

	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		return 1;

	uint32_t shader = CreateShader(g_VertexShaderSource, g_FragmentShaderSource);

	std::filesystem::path gltfPath = "res/this_tree_is_growing/scene.gltf";
	std::shared_ptr<nun::Model> nunModel = std::shared_ptr<nun::Model>(nun::LoadModelFromFile(gltfPath));
	Model model(*nunModel);
	mml::mat4 projection = mml::Perspective(60.0f, g_Window->GetAspectRatio(), 0.1f, 100000.0f);

	glEnable(GL_DEPTH_TEST);

	auto currentTime = std::chrono::high_resolution_clock::now();
	auto lastTime = currentTime;

	Camera camera(mml::vec3(0.0f, 0.0f, -3.0f), mml::vec3(0.0f, 0.0f, 1.0f));

	while (!g_Window->Closed())
	{
		currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
		lastTime = currentTime;

		g_LastMousePosition = g_MousePosition;
		g_Window->Update();
		camera.Update(deltaTime);

		glViewport(0, 0, g_Window->GetWidth(), g_Window->GetHeight());
		glClearColor(0.1f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		int modelLocation = glGetUniformLocation(shader, "u_Model");
		int viewLocation = glGetUniformLocation(shader, "u_View");
		int projectionLocation = glGetUniformLocation(shader, "u_Projection");

		glUniformMatrix4fv(viewLocation, 1, GL_TRUE, &camera.ViewMatrix[0][0]);
		glUniformMatrix4fv(projectionLocation, 1, GL_TRUE, &projection[0][0]);

		for (const auto& mesh : model.Meshes)
		{
			glBindVertexArray(mesh.VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);

			glUniformMatrix4fv(modelLocation, 1, GL_TRUE, &mesh.modelMatrix[0][0]);

			glDrawElements(mesh.drawType, mesh.indexCount, mesh.indexType, nullptr);
		}

		g_Window->SwapBuffers();
	}

	return 0;
}