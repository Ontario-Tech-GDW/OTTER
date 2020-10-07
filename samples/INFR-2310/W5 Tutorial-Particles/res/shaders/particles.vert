/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

particles.vert
Vertex shader.
Passes view-space particle position and color to the geometry shader.
*/

#version 420 core

//We expect this position is passed in view space.
//(Modelview multiplication happens on CPU for sorting.)
layout(location = 0) in vec4 inPos;
layout(location = 1) in float inSize;
layout(location = 2) in vec4 inColor;

out Vertex
{
    float size;
    vec4 color;
} v;

void main()
{
    v.size = inSize;
    v.color = inColor;
    gl_Position = inPos;
}