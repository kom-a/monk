#include "math/vec3.h"

#include <cmath>

namespace monk::math
{

	vec3::vec3()
		: x(0), y(0), z(0)
	{

	}

	vec3::vec3(float scalar)
		: x(scalar), y(scalar), z(scalar)
	{

	}

	vec3::vec3(float x, float y, float z)
		: x(x), y(y), z(z)
	{

	}

	vec3::vec3(const vec3& other)
		: x(other.x), y(other.y), z(other.z)
	{
	}

	vec3& vec3::operator=(const vec3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	vec3 vec3::operator+(const vec3& other) const
	{
		return vec3(x + other.x, y + other.y, z + other.z);
	}

	vec3 vec3::operator-(const vec3& other) const
	{
		return vec3(x - other.x, y - other.y, z - other.z);
	}

	float vec3::operator*(const vec3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	vec3 vec3::operator*(float scalar) const
	{
		return vec3(x * scalar, y * scalar, z * scalar);
	}

	vec3 vec3::operator/(float scalar) const
	{
		return vec3(x / scalar, y / scalar, z / scalar);
	}

	vec3& vec3::operator+=(const vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	vec3& vec3::operator-=(const vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	vec3& vec3::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	vec3& vec3::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	float& vec3::operator[](int index)
	{
		return *(((float*) this) + index);
	}

	const float& vec3::operator[](int index) const
	{
		return *(((float*)this) + index);
	}

	float vec3::length() const
	{
		return std::sqrt(length2());
	}

	float vec3::length2() const
	{
		return x * x + y * y + z * z;
	}

	bool vec3::operator==(const vec3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool vec3::operator!=(const vec3& other) const
	{
		return !(*this == other);
	}

	void vec3::normalize()
	{
		*this /= length();
	}
}