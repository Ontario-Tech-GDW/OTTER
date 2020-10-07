/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

particles.geom
Geometry shader.
Emits triangle strips for particles based on input view-space position.
Particles are billboarded.
*/

#version 420 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in Vertex
{
    float size;
    vec4 color;
} v[];

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec4 outColor;

uniform mat4 proj;

void main()
{
    vec4 p = gl_in[0].gl_Position;

    //Top left.
    vec2 v_tl = p.xy + vec2(-0.5f, 0.5f) * v[0].size;
    gl_Position = proj * vec4(v_tl, p.zw);
    outUV = vec2(0.0f, 1.0f);
    outColor = v[0].color;
    EmitVertex();

    //Bottom left.
    vec2 v_bl = p.xy + vec2(-0.5f, -0.5f) * v[0].size;
    gl_Position = proj * vec4(v_bl, p.zw);
    outUV = vec2(0.0f, 0.0f);
    outColor = v[0].color;
    EmitVertex();

    //Top right.
    vec2 v_tr = p.xy + vec2(0.5f, 0.5f) * v[0].size;
    gl_Position = proj * vec4(v_tr, p.zw);
    outUV = vec2(1.0f, 1.0f);
    outColor = v[0].color;
    EmitVertex();

    //Bottom right.
    vec2 v_br = p.xy + vec2(0.5f, -0.5f) * v[0].size;
    gl_Position = proj * vec4(v_br, p.zw);
    outUV = vec2(1.0f, 0.0f);
    outColor = v[0].color;
    EmitVertex();

    EndPrimitive();
}