#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ProjectionView;

out vec3 v_Color;

void main()
{
	gl_Position = u_ProjectionView * vec4(a_Position, 1.0f);
	v_Color = normalize(a_Position);
}