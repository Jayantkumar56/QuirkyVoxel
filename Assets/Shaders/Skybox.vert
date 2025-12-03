#version 460 core

layout (location = 0) in vec3 i_Pos;

layout (location = 0) out vec3 o_WorldPos;

uniform mat4 u_ViewProjection;

void main()
{
    o_WorldPos = i_Pos;
    
    vec4 clipPos = u_ViewProjection * vec4(i_Pos, 1.0);

    // OPTIMIZATION: Force Z to W.
    // After perspective division (Z/W), depth will be 1.0 (the far plane).
    gl_Position = clipPos.xyww;
}