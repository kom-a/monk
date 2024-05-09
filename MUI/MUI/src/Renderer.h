#pragma once

namespace mui
{
	struct Vec2i
	{
		int X = 0;
		int Y = 0;

		Vec2i(int x, int y)
			: X(x), Y(y) {}
	};

	struct Vec4i
	{
		int X = 0;
		int Y = 0;
		int Z = 0;
		int W = 0;

		Vec4i(int x, int y, int z, int w)
			: X(x), Y(y), Z(z), W(w) {}
	};



	class Renderer
	{
	public:
		Renderer();

		void DrawRect(Vec2i position, Vec2i size, Vec4i color);

	private:
		unsigned int m_Shader = 0;
	};
}