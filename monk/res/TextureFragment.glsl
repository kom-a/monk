#version 330 core

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D tex;

void main()
{
    float c = texture(tex, v_TexCoord).r;
    FragColor = vec4(c, c, c, 1.0f);
}