#include "math/Math.h"

#define _USE_MATH_DEFINES
#include <cmath>

#define TO_RADIANS(deg) (deg * M_PI / 180.0f)

namespace monk::math
{
	float Dot(const vec2& left, const vec2& right)
	{
		return left.x * right.x + left.y * right.y;
	}

	float Dot(const vec3& left, const vec3& right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}

	vec3 Cross(const vec3& left, const vec3& right)
	{
		float x = left.y * right.z - left.z * right.y;
		float y = left.z * right.x - left.x * right.z;
		float z = left.x * right.y - left.y * right.x;

		return vec3(x, y, z);
	}

	vec2 Normalize(const vec2& vector)
	{
		return vector / vector.length();
	}

	vec3 Normalize(const vec3& vector)
	{
		return vector / vector.length();
	}

	mat4 Ortho(float left, float right, float bottom, float top, float near, float far)
	{
		float a00 = 2.0f / (right - left);
		float a01 = 0.0f;
		float a02 = 0.0f;
		float a03 = -(right + left) / (right - left);

		float a10 = 0.0f;
		float a11 = 2 / (top - bottom);
		float a12 = 0.0f;
		float a13 = -(top + bottom) / (top - bottom);

		float a20 = 0.0f;
		float a21 = 0.0f;
		float a22 = -2 / (far - near);
		float a23 = -(far + near) / (far - near);

		float a30 = 0.0f;
		float a31 = 0.0f;
		float a32 = 0.0f;
		float a33 = 1.0f;

		return mat4(a00, a01, a02, a03,
			a10, a11, a12, a13,
			a20, a21, a22, a23,
			a30, a31, a32, a33);
	}

	mat4 Perspective(float fov, float aspect, float near, float far)
	{
		mat4 result;
		float rad = (float)TO_RADIANS(fov);
		float s = 1.0f / (std::tanf(rad / 2.0f));
		float z = near - far;

		result[0][0] = s / aspect;
		result[1][1] = s;
		result[2][2] = -(near + far) / z;
		result[2][3] = 2.0f * far * near / z;
		result[3][2] = 1.0f;
		
		return result;
	}

	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;
	}

	math::vec2 Clamp(const math::vec2& vector, const math::vec2& min, const math::vec2& max)
	{
		math::vec2 result;
		result.x = Clamp(vector.x, min.x, max.x);
		result.y = Clamp(vector.y, min.y, max.y);

		return result;
	}

	math::vec3 Clamp(const math::vec3& vector, const math::vec3& min, const math::vec3& max)
	{
		math::vec3 result;
		result.x = Clamp(vector.x, min.x, max.x);
		result.y = Clamp(vector.y, min.y, max.y);
		result.z = Clamp(vector.z, min.z, max.z);

		return result;
	}

	math::vec4 Clamp(const math::vec4& vector, const math::vec4& min, const math::vec4& max)
	{
		math::vec4 result;
		result.x = Clamp(vector.x, min.x, max.x);
		result.y = Clamp(vector.y, min.y, max.y);
		result.z = Clamp(vector.z, min.z, max.z);
		result.w = Clamp(vector.w, min.w, max.w);

		return result;
	}

}