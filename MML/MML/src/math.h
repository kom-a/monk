#pragma once

#include"vec2.h"
#include"vec3.h"
#include"mat4.h"

namespace mml
{
	float Dot(const vec2& left, const vec2& right);
	float Dot(const vec3& left, const vec3& right);
	vec3 Cross(const vec3& left, const vec3& right);
	mat4 Transpose(const mat4& mat);

	float ToRadians(float degrees);
	
	vec2 Normalize(const vec2& vector);
	vec3 Normalize(const vec3& vector);

	mat4 Ortho(float left, float right, float bottom, float top, float near, float far);
	mat4 Perspective(float fov, float aspect, float near, float far);

	float Clamp(float value, float min, float max);
	vec2 Clamp(const vec2& vector, const vec2& min, const vec2& max);
	vec3 Clamp(const vec3& vector, const vec3& min, const vec3& max);
	vec4 Clamp(const vec4& vector, const vec4& min, const vec4& max);

	float Min(float a, float b);
	float Max(float a, float b);

	mat4 LookAt(const vec3& position, const vec3& target);

	float Lerp(float a, float b, float t);
	vec2 Lerp(const vec2& a, const vec2& b, float t);
	vec3 Lerp(const vec3& a, const vec3& b, float t);
	vec4 Lerp(const vec4& a, const vec4& b, float t);
}