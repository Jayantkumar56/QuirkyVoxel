#version 460 core

// Per-Vertex Attributes
layout (location = 0) in uint a_Data1;
layout (location = 1) in uint a_Data2;

layout (location = 0) flat out uint v_TexIdx;
layout (location = 1) out vec2 v_TexCoord;

// --- Uniforms ---
uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

layout(std430, binding = 1) readonly buffer ChunkOffsetBuffer
{
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

void main()
{
    // --- 1. Unpack Data1 (Position) ---
    // We use bitwise AND (&) with a mask to isolate bits,
    // and bitwise right-shift (>>) to move them.

    // X: 5 bits (0-4)
    // Mask: 0x1F  (binary 11111)
    float x = float(a_Data1 & 0x1Fu);

    // Y: 5 bits (5-9)
    // Shift: 5, Mask: 0x1F
    float y = float((a_Data1 >> 5) & 0x1Fu);

    // Z: 5 bits (10-14)
    // Shift: 10, Mask: 0x1F
    float z = float((a_Data1 >> 10) & 0x1Fu);

    // Y Offset: 4 bits (15-18)
    // Shift: 15, Mask: 0x0F (binary 1111)
    // Divide by 16.0 for 1/16th precision
    float yOffset = float((a_Data1 >> 15) & 0x0Fu) / 16.0;

    // Combine into final local-space position
    vec3 localPos = vec3(x, y + yOffset, z);


    // --- 2. Unpack Data2 (Attributes) ---

    // Normal ID: 3 bits (0-2)
    // Mask: 0x07 (binary 111)
    uint normalID = a_Data2 & 0x07u;

    // Packed U: 5 bits (3-7)
    // Shift: 3, Mask: 0x1F
    // Divide by 16.0 (since your data is 0-16, not 0-31)
    float u = float((a_Data2 >> 3) & 0x1Fu);

    // Packed V: 5 bits (8-12)
    // Shift: 8, Mask: 0x1F
    // Divide by 16.0
    float v = float((a_Data2 >> 8) & 0x1Fu);

    // Texture ID: 10 bits (13-22)
    // Shift: 13, Mask: 0x3FF (binary 11 1111 1111)
    uint texID = (a_Data2 >> 13) & 0x3FFu;

    // Look up the normal from our constant array
    vec3 normal = k_Normals[normalID];

    v_TexIdx   = texID;
    v_TexCoord = vec2(u, v);

    // Standard Transformations
    vec4 worldPos = vec4(localPos + u_ChunkOffsets[gl_DrawID].xyz, 1.0);
    gl_Position = u_ViewProjection * worldPos;
}