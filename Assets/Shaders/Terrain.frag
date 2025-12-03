#version 460 core

out vec4 FragColor;

layout (location = 0) flat in uint v_TexIdx;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in vec3 v_FragPos;

uniform sampler2DArray u_BlockTextures;
uniform vec3 u_CameraPos;

// fixed global light direction (normalized)
uniform vec3 lightDir;

// fixed ambient and specular parameters
const float ambientStrength  = 0.3;
const float diffuseStrength  = 0.85;
const float specularStrength = 0.2;
const float shininess        = 16.0;

void main() {
    // Sample base color
    vec3 albedo = texture(u_BlockTextures, vec3(v_TexCoord, float(v_TexIdx))).rgb;

    // Normalize normal
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(lightDir); // light comes from lightDir

    // Simple ambient + diffuse + specular
    vec3 ambient  = ambientStrength * albedo;

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diffuseStrength * diff * albedo;

    // Assume view direction is from above (no actual camera vector)
    vec3 viewDir = normalize(u_CameraPos - v_FragPos); 
    vec3 reflectDir = reflect(-L, N);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * vec3(1.0);

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.0);
}
