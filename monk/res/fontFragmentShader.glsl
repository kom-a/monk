#version 330 core

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D sampler;

void main()
{
   FragColor = texture(sampler, v_TexCoord);
   
   // FragColor = vec4(1.0f);
}