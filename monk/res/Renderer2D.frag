#version 330 core 

out vec4 FragColor;

in vec4 v_Color;
in float v_TextureID;
in vec2 v_TextureCoords;

void main()
{
	FragColor = v_Color;
}