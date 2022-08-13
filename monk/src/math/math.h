#pragma once

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/mat4.h"

namespace monk::math
{
	float Dot(const vec2& left, const vec2& right);
	float Dot(const vec3& left, const vec3& right);
	vec3 Cross(const vec3& left, const vec3& right);
	
	vec2 Normalize(const vec2& vector);
	vec3 Normalize(const vec3& vector);

	mat4 Ortho(float left, float right, float bottom, float top, float near, float far);
	mat4 Perspective(float fov, float aspect, float near, float far);
}