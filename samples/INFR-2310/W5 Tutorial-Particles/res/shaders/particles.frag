/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

particles.frag
Fragment shader.
Samples colour from a given albedo texture without any lighting.
*/

#version 420 core

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outColor;

uniform sampler2D albedo;

void main()
{
    vec4 texCol = texture(albedo, inUV);

    outColor = vec4(inColor * texCol);
}