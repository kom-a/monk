#pragma once

namespace monk::math
{
	struct vec4
	{
		union { float x; float r; };
		union { float y; float g; };
		union { float z; float b; };
		union { float w; float a; };

		vec4();
		vec4(float scalar);
		vec4(float x, float y, float z, float w);
		vec4(const vec4& other);
		vec4& operator=(const vec4& other);

		vec4 operator+(const vec4& other) const;
		vec4 operator-(const vec4& other) const;
		float operator*(const vec4& other) const;
		vec4 operator*(float scalar) const;
		vec4 operator/(float scalar)const;

		vec4& operator+=(const vec4& other);
		vec4& operator-=(const vec4& other);
		vec4& operator*=(float scalar);
		vec4& operator/=(float scalar);

		bool operator==(const vec4& other) const;
		bool operator!=(const vec4& other) const;

		float& operator[](int index);
		const float& operator[](int index) const;

		float length() const;
		float length2() const;
		void normalize();
	};
}