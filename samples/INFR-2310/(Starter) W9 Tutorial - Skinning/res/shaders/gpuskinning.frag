/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

gpuskinning.frag
Fragment shader.
Uses a fixed directional light grey light with only diffuse and ambient lighting.
You'll learn a lot about lighting in graphics - this shader just gives us a simple
way to make sure that everything looks right with our normals, etc.

This is pretty much identical to our other texturedlit.frag.
The default light direction is different, just to make it a bit
easier to see what we're doing for the skinning demos.
*/

#version 420 core

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec4 outColor;

uniform vec3 camPos;
uniform sampler2D albedo;

uniform vec3 matColor = vec3(1.0f, 1.0f, 1.0f);

uniform vec3 lightColor = vec3(0.9, 0.9, 0.9);
uniform vec3 lightDir = normalize(vec3(0.0, 0.0, -1.0));
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

    vec3 result = (ambient + diff) * matColor * texture(albedo, inUV).rgb;

    outColor = vec4(result, 1.0);
}