#version 460 core

// Per-Vertex Attributes
layout (location = 0) in uint i_Data1;
layout (location = 1) in uint i_Data2;

// Output variable
layout (location = 0) flat out uint o_TexIdx;
layout (location = 1) out vec2 o_TexCoord;
layout (location = 2) out vec3 o_Normal;
layout (location = 3) out vec3 o_FragPos;

// Uniforms
uniform mat4 u_ViewProjection;

layout(std430, binding = 1) readonly buffer ChunkOffsetBuffer {
    vec4 u_ChunkOffsets[]; // An array of all chunk positions
};

const vec3 k_Normals[6] = vec3[6](
    vec3( 1.0,  0.0,  0.0), // 0: Right (+X)
    vec3(-1.0,  0.0,  0.0), // 1: Left  (-X)
    vec3( 0.0,  1.0,  0.0), // 2: Top   (+Y)
    vec3( 0.0, -1.0,  0.0), // 3: Bottom(-Y)
    vec3( 0.0,  0.0,  1.0), // 4: Front (+Z)
    vec3( 0.0,  0.0, -1.0)  // 5: Back  (-Z)
);

void main() {
    // Unpack position and offset from i_Data1
    vec3 localPos = vec3(
        // ----- Base -----                   ----- Offset -----
        float( i_Data1        & 0x1Fu), 
        float((i_Data1 >> 5)  & 0x1Fu)      +  float((i_Data1 >> 15) & 0x0Fu) / 16.0, 
        float((i_Data1 >> 10) & 0x1Fu)
    );

    // Normal ID: 3 bits (0-2)                   in i_Data2
    o_Normal = k_Normals[i_Data2 & 0x07u];

    // Texture ID: 10 bits (13-22)               in i_Data2
    o_TexIdx   = (i_Data2 >> 13) & 0x3FFu;
    
    o_TexCoord = vec2(
        float((i_Data2 >> 3) & 0x1Fu),        // Packed U: 5 bits (3-7)      in i_Data2
        float((i_Data2 >> 8) & 0x1Fu)         // Packed V: 5 bits (8-12)     in i_Data2
    );

    // Standard Transformations
    gl_Position = u_ViewProjection * vec4(localPos + u_ChunkOffsets[gl_DrawID].xyz, 1.0);
    o_FragPos = localPos + u_ChunkOffsets[gl_DrawID].xyz;
}