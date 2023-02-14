#version 330 core

layout (location = 0) in vec3 a_Normal;
layout (location = 1) in vec3 a_Position;

out vec3 v_Color;

uniform mat4 u_ProjectionView;

void main()
{
	gl_Position = u_ProjectionView * vec4(a_Position, 1.0f);

	v_Color = a_Normal * 0.5f + 0.5f;
	//v_Color = a_Position + 0.5f;
}