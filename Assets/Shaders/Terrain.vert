#version 460 core

// Per-Vertex Attributes
layout (location = 0) in uint a_Data1;
layout (location = 1) in uint a_Data2;

// Output variable
layout (location = 0) flat out uint v_TexIdx;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;
layout (location = 3) out vec3 v_FragPos;

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
    // Unpack position and offset from a_Data1
    vec3 localPos = vec3(
        // ----- Base -----                   ----- Offset -----
        float( a_Data1        & 0x1Fu), 
        float((a_Data1 >> 5)  & 0x1Fu)      +  float((a_Data1 >> 15) & 0x0Fu) / 16.0, 
        float((a_Data1 >> 10) & 0x1Fu)
    );

    // Normal ID: 3 bits (0-2)                   in a_Data2
    v_Normal = k_Normals[a_Data2 & 0x07u];

    // Texture ID: 10 bits (13-22)               in a_Data2
    v_TexIdx   = (a_Data2 >> 13) & 0x3FFu;
    
    v_TexCoord = vec2(
        float((a_Data2 >> 3) & 0x1Fu),        // Packed U: 5 bits (3-7)      in a_Data2
        float((a_Data2 >> 8) & 0x1Fu)         // Packed V: 5 bits (8-12)     in a_Data2
    );

    // Standard Transformations
    gl_Position = u_ViewProjection * vec4(localPos + u_ChunkOffsets[gl_DrawID].xyz, 1.0);
    v_FragPos = localPos + u_ChunkOffsets[gl_DrawID].xyz;
}