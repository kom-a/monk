#pragma once

#include "math/vec3.h"
#include "math/vec4.h"

namespace monk::math
{
	class mat4
	{
	public:
		mat4();
		mat4(float diagonal);
		mat4(float a00, float a01, float a02, float a03,
			float a10, float a11, float a12, float a13,
			float a20, float a21, float a22, float a23,
			float a30, float a31, float a32, float a33);
		mat4(const mat4& other);
		mat4& operator=(const mat4& other);

		mat4 operator+(const mat4& other);
		mat4 operator-(const mat4& other);
		mat4 operator*(const mat4& other);
		mat4 operator*(float scalar);

		friend const mat4 operator+(const mat4& left, const mat4& right);
		friend const mat4 operator-(const mat4& left, const mat4& right);
		friend const mat4 operator*(const mat4& left, const mat4& right);
		friend const mat4 operator*(const mat4& left, float right);

		mat4& operator+=(const mat4& other);
		mat4& operator-=(const mat4& other);
		mat4& operator*=(const mat4& other);
		mat4& operator*=(float scalar);

		void Translate(const vec3& translation);
		void Rotate(float deg, vec3 axis);
		void Scale(const vec3& scale);

		vec4& operator[](int index);
		const vec4& operator[](int index) const;

	private:
		union
		{
			float m_Elements[4 * 4];
			vec4 m_Rows[4];
		};
	};
}


