#version 460 core

out vec4 FragColor;

layout (location = 0) flat in uint v_TexIdx;
layout (location = 1) in vec2 v_TexCoord;

uniform sampler2DArray u_BlockTextures;

void main()
{
    FragColor = texture(u_BlockTextures, vec3(v_TexCoord.xy, float(v_TexIdx)));
}