#version 330 core

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D tex;

void main()
{
    float sdf = texture(tex, v_TexCoord).r;

    if (sdf > 0.5f)
        FragColor = vec4(1.0f);
    else 
        discard;
}