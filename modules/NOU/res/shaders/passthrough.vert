/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

passthrough.vert
Vertex shader.
Passes unaltered vertex position to the fragment shader.
*/

#version 420 core

layout(location = 0) in vec4 vPos;

void main()
{
    gl_Position = vPos;
}