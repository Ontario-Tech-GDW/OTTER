/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

passthrough.frag
Fragment shader.
Sets fragment colour to white.
*/

#version 420 core

layout(location = 0) out vec4 col;

void main()
{
    col = vec4(1.0, 1.0, 1.0, 1.0);
}