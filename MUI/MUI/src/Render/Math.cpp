#include "Math.h"

namespace mui
{
	Vec2f::Vec2f()
		: X(0.0f), Y(0.0f)
	{ }

	Vec2f::Vec2f(float x, float y)
		: X(x), Y(y) {}


	Vec2f Vec2f::operator+(const Vec2f& other)
	{
		return Vec2f(X + other.X, Y + other.Y);
	}

	Vec2f& Vec2f::operator+=(const Vec2f& other)
	{
		X += other.X;
		Y += other.Y;

		return *this;
	}

	Vec2f Vec2f::operator-(const Vec2f& other)
	{
		return Vec2f(X - other.X, Y - other.Y);
	}

	Vec2f& Vec2f::operator-=(const Vec2f& other)
	{
		X -= other.X;
		Y -= other.Y;

		return *this;
	}

	Vec2f Vec2f::operator*(float scale)
	{
		return Vec2f(X * scale, Y * scale);
	}

	Vec2f& Vec2f::operator*=(float scale)
	{
		X *= scale;
		Y *= scale;

		return *this;
	}

	bool Vec2f::operator==(const Vec2f& other)
	{
		return X == other.X && Y == other.Y;
	}

	Vec3f::Vec3f()
		: X(0.0f), Y(0.0f), Z(0.0f)
	{ }

	Vec3f::Vec3f(float x, float y, float z)
		: X(x), Y(y), Z(z) {}

	Vec3f::Vec3f(const Vec2f& vec2, float z)
		: X(vec2.X), Y(vec2.Y), Z(z) {}

	Vec3f Vec3f::operator+(const Vec3f& other)
	{
		return Vec3f(X + other.X, Y + other.Y, Z + other.Z);
	}

	Vec3f& Vec3f::operator+=(const Vec3f& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;

		return *this;
	}

	Vec3f Vec3f::operator-(const Vec3f& other)
	{
		return Vec3f(X - other.X, Y - other.Y, Z - other.Z);
	}

	Vec3f& Vec3f::operator-=(const Vec3f& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;

		return *this;
	}

	Vec3f Vec3f::operator*(float scale)
	{
		return Vec3f(X * scale, Y * scale, Z * scale);
	}

	Vec3f& Vec3f::operator*=(float scale)
	{
		X *= scale;
		Y *= scale;
		Z *= scale;

		return *this;
	}

	bool Vec3f::operator==(const Vec3f& other)
	{
		return X == other.X && Y == other.Y && Z == other.Z;
	}

	Vec4f::Vec4f()
		: X(0.0f), Y(0.0f), Z(0.0f), W(0.0f)
	{ }

	Vec4f::Vec4f(float x, float y, float z, float w)
		: X(x), Y(y), Z(z), W(w) {}

	Vec4f::Vec4f(Vec3f vec3, float w)
		: X(vec3.X), Y(vec3.Y), Z(vec3.Z), W(w) {}

	Vec4f::Vec4f(Vec2f vec2, float z, float w)
		: X(vec2.X), Y(vec2.Y), Z(z), W(w) {}

	Vec4f Vec4f::operator+(const Vec4f& other)
	{
		return Vec4f(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
	}

	Vec4f& Vec4f::operator+=(const Vec4f& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		W += other.W;

		return *this;
	}

	Vec4f Vec4f::operator-(const Vec4f& other)
	{
		return Vec4f(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
	}

	Vec4f& Vec4f::operator-=(const Vec4f& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		W -= other.W;

		return *this;
	}

	Vec4f Vec4f::operator*(float scale)
	{
		return Vec4f(X * scale, Y * scale, Z * scale, W * scale);
	}

	Vec4f& Vec4f::operator*=(float scale)
	{
		X *= scale;
		Y *= scale;
		Z *= scale;
		W *= scale;

		return *this;
	}

	bool Vec4f::operator==(const Vec4f& other)
	{
		return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
	}

}

