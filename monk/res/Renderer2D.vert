#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in float a_TextureID;
layout (location = 3) in vec2 a_TextureCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec4 v_Color;
out float v_TextureID;
out vec2 v_TextureCoords;

void main()
{
	gl_Position = u_Projection * u_View * vec4(a_Position, 1.0f);

	v_Color = a_Color;
	v_TextureID = a_TextureID;
	v_TextureCoords = a_TextureCoords;
}