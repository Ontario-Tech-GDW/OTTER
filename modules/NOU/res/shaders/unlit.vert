/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

unlit.vert
Vertex shader.
Passes world vertex position to the fragment shader.
*/

#version 420 core

layout(location = 0) in vec4 inPos;

uniform mat4 model;
uniform mat4 viewproj;

void main()
{
    gl_Position = viewproj * model * inPos;
}