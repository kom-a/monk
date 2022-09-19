#version 330 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 u_Projection;

out vec2 v_TexCoord;

void main()
{
    gl_Position = u_Projection * vec4(vec3(a_Position, 1.0f), 1.0f);
    v_TexCoord = a_TexCoord;
}
