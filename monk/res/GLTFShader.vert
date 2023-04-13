#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_ProjectionView;
uniform mat4 u_Model;

out vec3 v_Color;

void main()
{
	gl_Position = u_ProjectionView * u_Model * vec4(a_Position, 1.0f);
	v_Color = normalize(a_Position);
}