#version 330 core 

out vec4 FragColor;

in vec4 v_Color;
in float v_TextureID;
in vec2 v_TextureCoords;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform sampler2D u_Texture4;
uniform sampler2D u_Texture5;
uniform sampler2D u_Texture6;
uniform sampler2D u_Texture7;
uniform sampler2D u_Texture8;
uniform sampler2D u_Texture9;
uniform sampler2D u_Texture10;
uniform sampler2D u_Texture11;
uniform sampler2D u_Texture12;
uniform sampler2D u_Texture13;
uniform sampler2D u_Texture14;
uniform sampler2D u_Texture15;

void main()
{
	int textureIndex = int(v_TextureID);
	switch(textureIndex)
	{
		case 0: FragColor = texture(u_Texture0, v_TextureCoords) * v_Color; break;
		case 1: FragColor = texture(u_Texture1, v_TextureCoords) * v_Color; break;
		case 2: FragColor = texture(u_Texture2, v_TextureCoords) * v_Color; break;
		case 3: FragColor = texture(u_Texture3, v_TextureCoords) * v_Color; break;
		case 4: FragColor = texture(u_Texture4, v_TextureCoords) * v_Color; break;
		case 5: FragColor = texture(u_Texture5, v_TextureCoords) * v_Color; break;
		case 6: FragColor = texture(u_Texture6, v_TextureCoords) * v_Color; break;
		case 7: FragColor = texture(u_Texture7, v_TextureCoords) * v_Color; break;
		case 8: FragColor = texture(u_Texture8, v_TextureCoords) * v_Color; break;
		case 9: FragColor = texture(u_Texture9, v_TextureCoords) * v_Color; break;
		case 10: FragColor = texture(u_Texture10, v_TextureCoords) * v_Color; break;
		case 11: FragColor = texture(u_Texture11, v_TextureCoords) * v_Color; break;
		case 12: FragColor = texture(u_Texture12, v_TextureCoords) * v_Color; break;
		case 13: FragColor = texture(u_Texture13, v_TextureCoords) * v_Color; break;
		case 14: FragColor = texture(u_Texture14, v_TextureCoords) * v_Color; break;
		case 15: FragColor = texture(u_Texture15, v_TextureCoords) * v_Color; break;
		default: FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	}
}