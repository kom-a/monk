#pragma once

namespace mui
{
	struct Vec2f
	{
		float X = 0;
		float Y = 0;

		Vec2f();
		Vec2f(float x, float y);

		Vec2f operator+(const Vec2f& other);
		Vec2f& operator+=(const Vec2f& other);

		Vec2f operator-(const Vec2f& other);
		Vec2f& operator-=(const Vec2f& other);

		Vec2f operator*(float scale);
		Vec2f& operator*=(float scale);

		bool operator==(const Vec2f& other);
	};

	struct Vec3f
	{
		float X = 0;
		float Y = 0;
		float Z = 0;

		Vec3f();
		Vec3f(float x, float y, float z);
		Vec3f(const Vec2f& vec2, float z);

		Vec3f operator+(const Vec3f& other);
		Vec3f& operator+=(const Vec3f& other);

		Vec3f operator-(const Vec3f& other);
		Vec3f& operator-=(const Vec3f& other);

		Vec3f operator*(float scale);
		Vec3f& operator*=(float scale);

		bool operator==(const Vec3f& other);
	};

	struct Vec4f
	{
		float X = 0;
		float Y = 0;
		float Z = 0;
		float W = 0;

		Vec4f();
		Vec4f(float x, float y, float z, float w);
		Vec4f(Vec3f vec3, float w);
		Vec4f(Vec2f vec2, float z, float w);

		Vec4f operator+(const Vec4f& other);
		Vec4f& operator+=(const Vec4f& other);

		Vec4f operator-(const Vec4f& other);
		Vec4f& operator-=(const Vec4f& other);

		Vec4f operator*(float scale);
		Vec4f& operator*=(float scale);

		bool operator==(const Vec4f& other);
	};
}