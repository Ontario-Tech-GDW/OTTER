/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

texturedunlit.vert
Vertex shader.
Passes world vertex position and UV coordinates to the fragment shader.
*/

#version 420 core

layout(location = 0) in vec4 inPos;
layout(location = 2) in vec2 inUV;

layout(location = 2) out vec2 outUV;

uniform mat4 model;
uniform mat4 viewproj;

void main()
{
    outUV = inUV;
    gl_Position = viewproj * model * inPos;
}