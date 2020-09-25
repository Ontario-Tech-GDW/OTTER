/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

texturedunlit.frag
Fragment shader.
Samples colour from a given albedo texture without any lighting.
*/

#version 420 core

layout(location = 2) in vec2 inUV;

layout(location = 0) out vec4 outColor;

uniform vec3 matColor = vec3(1.0f, 1.0f, 1.0f);
uniform sampler2D albedo;

void main()
{
    vec4 texCol = texture(albedo, inUV);

    outColor = vec4(matColor * texCol.rgb, texCol.a);
}