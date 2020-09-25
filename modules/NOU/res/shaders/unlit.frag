/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

unlit.frag
Fragment shader.
Outputs uniform colour without any lighting.
*/

#version 420 core

layout(location = 0) out vec4 outColor;

uniform vec3 matColor = vec3(1.0f, 1.0f, 1.0f);

void main()
{
    outColor = vec4(matColor, 1.0f);
}