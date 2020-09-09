/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

texturedlit.frag
Fragment shader.
Uses a fixed directional light grey light with only diffuse and ambient lighting.
You'll learn a lot about lighting in graphics - this shader just gives us a simple
way to make sure that everything looks right with our normals, etc.
*/

#version 460 core

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec4 outColor;

uniform vec3 camPos;

uniform vec3 matCol = vec3(1.0, 1.0, 1.0);
uniform sampler2D albedo;

uniform vec3 lightColor = vec3(0.9, 0.9, 0.9);
uniform vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));
uniform vec3 ambientColor = vec3(1.0, 1.0, 1.0);
uniform float ambientPower = 0.2;

void main()
{
    vec3 norm = normalize(inNorm); 

    vec3 eye = normalize(camPos - inPos.xyz);
    vec3 toLight = -lightDir;

    vec3 avg = normalize(eye + toLight);

    float diffPower = max(dot(norm, toLight), 0.0);
    vec3 diff = diffPower * lightColor;

    vec3 ambient = ambientPower * ambientColor;

    vec3 result = (ambient + diff) * matCol * texture(albedo, inUV).rgb;

    outColor = vec4(result, 1.0);
}