/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

morph.vert
Vertex shader.
Basic morph-based blending of vertex position and normals.
*/

#version 420 core

uniform mat4 model;
uniform mat3 normal;
uniform mat4 viewproj;
uniform float t;

//Keyframe 0 vertex position.
layout(location = 0) in vec4 inPos_0;
//Keyframe 1 vertex position.
layout(location = 1) in vec4 inPos_1;
//Keyframe 0 vertex normal.
layout(location = 2) in vec3 inNorm_0;
//Keyframe 1 vertex normal.
layout(location = 3) in vec3 inNorm_1;

//The vertex position and normal we will send to
//the fragment shader.
layout(location = 0) out vec4 outPos;
layout(location = 1) out vec3 outNorm;

void main()
{
    //TODO: What is our world-space vertex position?
    //(With morph target animation...?)
    outPos = model * inPos_0;

    //TODO: What is our world-space normal position?
    //(With morph target animation...?)
    outNorm = normal * inNorm_0;
    
    //Output position - our viewprojection matrix
    //multiplied by world-space position.
    gl_Position = viewproj * outPos;
}