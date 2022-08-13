#include "math/vec2.h"

#include <cmath>

namespace monk::math
{
	vec2::vec2()
		: x(0), y(0)
	{

	}

	vec2::vec2(float scalar)
		: x(scalar), y(scalar)
	{

	}

	vec2::vec2(float x, float y)
		: x(x), y(y)
	{

	}

	vec2::vec2(const vec2& other)
		: x(other.x), y(other.y)
	{
	}

	vec2& vec2::operator=(const vec2& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	vec2 vec2::operator+(const vec2& other) const
	{
		return vec2(x + other.x, y + other.y);
	}

	vec2 vec2::operator-(const vec2& other) const
	{
		return vec2(x - other.x, y - other.y);
	}

	float vec2::operator*(const vec2& other) const
	{
		return x * other.x + y * other.y;
	}

	vec2 vec2::operator*(float scalar) const
	{
		return vec2(x * scalar, y * scalar);
	}

	vec2 vec2::operator/(float scalar) const
	{
		return vec2(x / scalar, y / scalar);
	}

	vec2& vec2::operator+=(const vec2& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	vec2& vec2::operator-=(const vec2& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	vec2& vec2::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	vec2& vec2::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	bool vec2::operator==(const vec2& other) const
	{
		return x == other.x && y == other.y;
	}

	bool vec2::operator!=(const vec2& other) const
	{
		return !(*this == other);
	}

	float& vec2::operator[](int index)
	{
		return *(((float*)this) + index);
	}

	const float& vec2::operator[](int index) const
	{
		return *(((float*)this) + index);
	}

	float vec2::length() const
	{
		return std::sqrt(length2());
	}

	float vec2::length2() const
	{
		return x * x + y * y;
	}

	void vec2::normalize()
	{
		*this /= length();
	}
}