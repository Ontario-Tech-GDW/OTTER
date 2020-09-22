/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

texturedlit.vert
Vertex shader.
Passes world vertex position, transformed normal direction, and UV coordinates
to the fragment shader.
*/

#version 420 core

uniform mat4 model;
uniform mat3 normal;
uniform mat4 viewproj;

layout(location = 0) in vec4 inPos;
layout(location = 1) in vec3 inNorm;

layout(location = 0) out vec4 outPos;
layout(location = 1) out vec3 outNorm;

void main()
{
    outNorm = normal * inNorm;
    outPos = model * inPos;

    gl_Position = viewproj * outPos;
}