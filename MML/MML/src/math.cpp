#include"Math.h"

#define _USE_MATH_DEFINES
#include <cmath>

#define TO_RADIANS(deg) (deg * M_PI / 180.0f)

namespace mml
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

	vec2 Clamp(const vec2& vector, const vec2& min, const vec2& max)
	{
		vec2 result;
		result.x = Clamp(vector.x, min.x, max.x);
		result.y = Clamp(vector.y, min.y, max.y);

		return result;
	}

	vec3 Clamp(const vec3& vector, const vec3& min, const vec3& max)
	{
		vec3 result;
		result.x = Clamp(vector.x, min.x, max.x);
		result.y = Clamp(vector.y, min.y, max.y);
		result.z = Clamp(vector.z, min.z, max.z);

		return result;
	}

	vec4 Clamp(const vec4& vector, const vec4& min, const vec4& max)
	{
		vec4 result;
		result.x = Clamp(vector.x, min.x, max.x);
		result.y = Clamp(vector.y, min.y, max.y);
		result.z = Clamp(vector.z, min.z, max.z);
		result.w = Clamp(vector.w, min.w, max.w);

		return result;
	}

	float Min(float a, float b)
	{
		return a < b ? a : b;
	}

	float Max(float a, float b)
	{
		return a > b ? a : b;
	}

	mat4 LookAt(const vec3& position, const vec3& target)
	{
		vec3 cameraDirection = Normalize(target - position);
		vec3 up = vec3(0.0f, 1.0f, 0.0f);
		vec3 cameraRight = Normalize(Cross(up, cameraDirection));
		vec3 cameraUp = Cross(cameraDirection, cameraRight);

		const vec3& R = cameraRight;
		const vec3& U = cameraUp;
		const vec3& D = cameraDirection;
		const vec3& P = position;

		return mat4(
			R.x, R.y, R.z, R.x * -P.x + R.y * -P.y + R.z * -P.z,
			U.x, U.y, U.z, U.x * -P.x + U.y * -P.y + U.z * -P.z,
			D.x, D.y, D.z, D.x * -P.x + D.y * -P.y + D.z * -P.z,
			0, 0, 0, 1);
	}

	float ToRadians(float degrees)
	{
		return (float)TO_RADIANS(degrees);
	}

	mat4 Transpose(const mat4& mat)
	{
		return mat4(
			mat[0][0], mat[1][0], mat[2][0], mat[3][0],
			mat[0][1], mat[1][1], mat[2][1], mat[3][1],
			mat[0][2], mat[1][2], mat[2][2], mat[3][2],
			mat[0][3], mat[1][3], mat[2][3], mat[3][3] 
		);
	}
}