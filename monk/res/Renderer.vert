#version 330 core

layout (location = 0) in vec3 a_Position;

out vec3 v_Color;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	gl_Position = u_Projection * u_View * vec4(a_Position, 1.0f);

	v_Color = (a_Position + 0.5f);
}