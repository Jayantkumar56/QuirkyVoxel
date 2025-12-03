#version 450 core

out vec4 o_FragColor;

layout (location = 0) in vec3 v_WorldPos;

uniform vec3  u_SunDir;
uniform float u_SunRadius;
uniform vec3  u_MoonDir;
uniform float u_MoonRadius;

uniform float u_SunriseSkyWeight;
uniform float u_DaySkyWeight;
uniform float u_NightSkyWeight;
uniform float u_MidnightSkyWeight;

uniform float u_SunVisibility;
uniform float u_MoonVisibility;

uniform sampler2D u_SunriseSkyGradient;
uniform sampler2D u_DaySkyGradient;
uniform sampler2D u_NightSkyGradient;
uniform sampler2D u_MidnightSkyGradient;

uniform sampler2D u_MoonTexture;


const vec3 u_SunTint = vec3(1.0, 0.98, 0.92);

// make the sun edge softer
const float u_SunSoftEdgeDeg  = 0.15;


vec4 GetSkyColor(vec3 fragDir) {
    const vec2 texCoord = vec2((fragDir.y + 1.0) * 0.5, 0.5);

    const vec4 colorMidnight = texture(u_MidnightSkyGradient, texCoord);
    const vec4 colorSunrise  = texture(u_SunriseSkyGradient,  texCoord);
    const vec4 colorDay      = texture(u_DaySkyGradient,      texCoord);
    const vec4 colorNight    = texture(u_NightSkyGradient,    texCoord);

    vec4 skyColor = 
        (colorMidnight * u_MidnightSkyWeight) +
        (colorSunrise  * u_SunriseSkyWeight ) +
        (colorDay      * u_DaySkyWeight     ) +
        (colorNight    * u_NightSkyWeight   );

    return skyColor;
}

float GetSunAlpha(vec3 fragDir) {
    const float cosTheta = dot(u_SunDir, fragDir);
    const float cosR0    = cos(u_SunRadius);
    const float cosR1    = cos(u_SunRadius + radians(u_SunSoftEdgeDeg));
    return smoothstep(cosR1, cosR0, cosTheta);
}

vec2 GetMoonUV(vec3 fragDir) {
    vec3 up    = normalize(u_MoonDir);
    vec3 right = vec3(0.0, 0.0, 1.0);
    vec3 fwd   = cross(up, right);
    
    float x = dot(fragDir, right);
    float z = dot(fragDir, fwd);
    
    float scale = tan(u_MoonRadius);
    vec2 uv = vec2(x, z) / scale;
    
    return uv * 0.5 + 0.5;
}

void main() {
    const vec3  fragDir  = normalize(v_WorldPos);

    vec4  skyColor = GetSkyColor(fragDir);

    if (dot(fragDir, u_MoonDir) > cos(u_MoonRadius)) {
        vec2 moonUV    = GetMoonUV(fragDir);
        vec4 moonColor = vec4(texture(u_MoonTexture, moonUV).xyz, u_MoonVisibility);

        skyColor = mix(skyColor, moonColor, moonColor.a);
    }

    const float sunAlpha = u_SunVisibility * GetSunAlpha(fragDir);

    o_FragColor = vec4((1 - sunAlpha) * skyColor.xyz + sunAlpha * u_SunTint, 1.0);
}