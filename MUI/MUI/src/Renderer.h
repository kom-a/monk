#pragma once

#include <vector>

namespace mui
{
	struct Vec2f
	{
		float X = 0;
		float Y = 0;

		Vec2f(float x, float y)
			: X(x), Y(y) {}
	};

	struct Vec3f
	{
		float X = 0;
		float Y = 0;
		float Z = 0;

		Vec3f(float x, float y, float z)
			: X(x), Y(y), Z(z) {}
	};

	struct Vec4f
	{
		float X = 0;
		float Y = 0;
		float Z = 0;
		float W = 0;

		Vec4f(float x, float y, float z, float w)
			: X(x), Y(y), Z(z), W(w) {}
	};

	struct Vertex
	{
		Vec3f Position;
		Vec4f Color; 

		Vertex(Vec3f position, Vec4f color) :
			Position(position), Color(color) { };
	};

	class Renderer
	{
	public:
		Renderer();

		void BeginDraw();
			void DrawRect(Vec2f position, Vec2f size, Vec4f color);
		void EndDraw();

		void Flush();

	private:
		uint32_t CreateShader() const;
		uint32_t CreateVertexArrayObject() const;
		uint32_t CreateVertexBufferObject() const;


	private:
		uint32_t m_Shader	= 0;
		uint32_t m_VAO		= 0;
		uint32_t m_VBO		= 0;

		struct RenderSettings
		{
			static uint32_t MaxVertices;
		};

		struct RenderState
		{
			uint32_t VerticesCount = 0;
		};

		std::vector<Vertex> m_Buffer;

		RenderState m_RenderState;


	};
}