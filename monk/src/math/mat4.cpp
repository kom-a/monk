#include "Mat4.h"

#define _USE_MATH_DEFINES
#include <cmath>

#define TO_RADIANS(deg) (deg * M_PI / 180.0f)

namespace monk::math
{
	mat4::mat4()
	{
		for (int i = 0; i < 4 * 4; i++)
			m_Elements[i] = 0;
	}

	mat4::mat4(float diagonal)
	{
		for (int row = 0; row < 4; row++)
			for (int col = 0; col < 4; col++)
				if (row == col)
					m_Elements[row * 4 + col] = diagonal;
				else
					m_Elements[row * 4 + col] = 0.0f;
	}

	mat4::mat4(const mat4& other)
	{
		for (int i = 0; i < 4 * 4; i++)
			m_Elements[i] = other.m_Elements[i];
	}

	mat4::mat4(float a00, float a01, float a02, float a03, 
		float a10, float a11, float a12, float a13, 
		float a20, float a21, float a22, float a23, 
		float a30, float a31, float a32, float a33)
	{
		m_Elements[0 * 4 + 0] = a00;
		m_Elements[0 * 4 + 1] = a01;
		m_Elements[0 * 4 + 2] = a02;
		m_Elements[0 * 4 + 3] = a03;

		m_Elements[1 * 4 + 0] = a10;
		m_Elements[1 * 4 + 1] = a11;
		m_Elements[1 * 4 + 2] = a12;
		m_Elements[1 * 4 + 3] = a13;

		m_Elements[2 * 4 + 0] = a20;
		m_Elements[2 * 4 + 1] = a21;
		m_Elements[2 * 4 + 2] = a22;
		m_Elements[2 * 4 + 3] = a23;

		m_Elements[3 * 4 + 0] = a30;
		m_Elements[3 * 4 + 1] = a31;
		m_Elements[3 * 4 + 2] = a32;
		m_Elements[3 * 4 + 3] = a33;
	}

	mat4& mat4::operator=(const mat4& other)
	{
		for (int i = 0; i < 4 * 4; i++)
			m_Elements[i] = other.m_Elements[i];

		return *this;
	}

	mat4 mat4::operator+(const mat4& other)
	{
		mat4 result = *this;

		for (int row = 0; row < 4; row++)
			for (int col = 0; col < 4; col++)
				result[row * 4 + col] += other[row * 4 + col];

		return result;
	}

	mat4 mat4::operator-(const mat4& other)
	{
		mat4 result = *this;

		for (int row = 0; row < 4; row++)
			for (int col = 0; col < 4; col++)
				result.m_Elements[row * 4 + col] -= other.m_Elements[row * 4 + col];

		return result;
	}

	mat4 mat4::operator*(const mat4& other)
	{
		mat4 tmp = *this;

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				float sum = 0.0f;
				for (int e = 0; e < 4; e++)
					sum += m_Elements[e + row * 4] * other.m_Elements[col + e * 4];

				tmp.m_Elements[col + row * 4] = sum;
			}
		}

		return tmp;
	}

	mat4 mat4::operator*(float scalar)
	{
		mat4 result = *this;

		for (int i = 0; i < 4 * 4; i++)
			result[i] *= scalar;

		return result;
	}

	mat4& mat4::operator+=(const mat4& other)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m_Elements[i * 4 + j] += other.m_Elements[i * 4 + j];

		return *this;
	}

	mat4& mat4::operator-=(const mat4& other)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				m_Elements[i * 4 + j] -= other.m_Elements[i * 4 + j];

		return *this;
	}

	mat4& mat4::operator*=(const mat4& other)
	{
		mat4 tmp = *this;

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				float sum = 0.0f;
				for (int e = 0; e < 4; e++)
					sum += m_Elements[e + row * 4] * other.m_Elements[col + e * 4];

				tmp.m_Elements[col + row * 4] = sum;
			}
		}

		*this = tmp;
		return *this;
	}

	mat4& mat4::operator*=(float scalar)
	{
		for (int i = 0; i < 4 * 4; i++)
			m_Elements[i] *= scalar;

		return *this;
	}

	void mat4::Translate(const vec3& translation)
	{
		m_Elements[0 * 4 + 3] += translation.x;
		m_Elements[1 * 4 + 3] += translation.y;
		m_Elements[2 * 4 + 3] += translation.z;
	}

	void mat4::Rotate(float deg, vec3 axis)
	{
		axis.normalize();

		const float& x = axis.x;
		const float& y = axis.y;
		const float& z = axis.z;

		float theta = (float)TO_RADIANS(deg);
		float cos = std::cos(theta);
		float sin = std::sin(theta);
		float omc = 1 - cos;

		float a00 = cos + x * omc;
		float a01 = x * y * omc - z * sin;
		float a02 = x * z * omc + y * sin;
		float a03 = 0.0f;

		float a10 = y * x * omc + z * sin;
		float a11 = cos + y * y * omc;
		float a12 = y * z * omc - x * sin;
		float a13 = 0.0f;

		float a20 = z * x * omc - y * sin;
		float a21 = z * y * omc + x * sin;
		float a22 = cos + z * z * omc;
		float a23 = 0.0f;

		float a30 = 0.0f;
		float a31 = 0.0f;
		float a32 = 0.0f;
		float a33 = 1.0f;

		mat4 rotation(a00, a01, a02, a03,
			a10, a11, a12, a13,
			a20, a21, a22, a23,
			a30, a31, a32, a33);

		*this *= rotation;
	}

	void mat4::Scale(const vec3& scale)
	{
		m_Elements[0 * 4 + 0] *= scale.x;
		m_Elements[1 * 4 + 1] *= scale.y;
		m_Elements[2 * 4 + 2] *= scale.z;
	}

	vec4& mat4::operator[](int index)
	{
		return m_Rows[index];
	}

	const vec4& mat4::operator[](int index) const
	{
		return m_Rows[index];
	}

}