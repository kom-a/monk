#include "math/Vec4.h"

#include <cmath>

namespace monk::math
{
	vec4::vec4()
		: x(0), y(0), z(0), w(0)
	{

	}

	vec4::vec4(float scalar)
		: x(scalar), y(scalar), z(scalar), w(scalar)
	{

	}

	vec4::vec4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w)
	{

	}

	vec4::vec4(const vec4& other)
		: x(other.x), y(other.y), z(other.z), w(other.w)
	{
	
	}

	vec4& vec4::operator=(const vec4& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;

		return *this;
	}

	vec4 vec4::operator+(const vec4& other) const
	{
		return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	vec4 vec4::operator-(const vec4& other) const
	{
		return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	float vec4::operator*(const vec4& other) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

	vec4 vec4::operator*(float scalar) const
	{
		return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	vec4 vec4::operator/(float scalar) const
	{
		return vec4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	vec4& vec4::operator+=(const vec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	vec4& vec4::operator-=(const vec4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	vec4& vec4::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	vec4& vec4::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;

		return *this;
	}

	bool vec4::operator==(const vec4& other) const
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool vec4::operator!=(const vec4& other) const
	{
		return !(*this == other);
	}

	float& vec4::operator[](int index)
	{
		return *(((float*)this) + index);
	}

	const float& vec4::operator[](int index) const
	{
		return *(((float*)this) + index);
	}

	float vec4::length() const
	{
		return std::sqrt(length2());
	}

	float vec4::length2() const
	{
		return x * x + y * y + z * z + w * w;
	}

	void vec4::normalize()
	{
		*this /= length();
	}
}