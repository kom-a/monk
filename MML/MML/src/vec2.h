#pragma once

namespace mml
{
	struct vec2
	{
		union
		{
			struct { float x, y; };
			struct { float s, t; };
		};
		
		vec2();
		vec2(float scalar);
		vec2(float x, float y);
		vec2(const vec2& other);
		vec2& operator=(const vec2& other);

		vec2 operator+(const vec2& other) const;
		vec2 operator-(const vec2& other) const;
		float operator*(const vec2& other) const;
		vec2 operator*(float scalar) const;
		vec2 operator/(float scalar) const;

		vec2& operator+=(const vec2& other);
		vec2& operator-=(const vec2& other);
		vec2& operator*=(float scalar);
		vec2& operator/=(float scalar);

		vec2 operator-() const;

		bool operator==(const vec2& other) const;
		bool operator!=(const vec2& other) const;

		float& operator[](int index);
		const float& operator[](int index) const;

		float length() const;
		float length2() const;
		void normalize();
	};
}