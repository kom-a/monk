#pragma once

namespace mmath
{
	struct vec3
	{
		union { float x; float r; };
		union { float y; float g; };
		union { float z; float b; };

		vec3();
		vec3(float scalar);
		vec3(float x, float y, float z);
		vec3(const vec3& other);
		vec3& operator=(const vec3& other);

		vec3 operator+(const vec3& other) const;
		vec3 operator-(const vec3& other) const;
		float operator*(const vec3& other) const;
		vec3 operator*(float scalar) const;
		vec3 operator/(float scalar)const;

		vec3& operator+=(const vec3& other);
		vec3& operator-=(const vec3& other);
		vec3& operator*=(float scalar);
		vec3& operator/=(float scalar);

		vec3 operator+() const;
		vec3 operator-() const;

		bool operator==(const vec3& other) const;
		bool operator!=(const vec3& other) const;

		float& operator[](int index);
		const float& operator[](int index) const;

		float length() const;
		float length2() const;
		void normalize();
	};
}