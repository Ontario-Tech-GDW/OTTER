/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

gpuskinning.vert
Vertex shader.
For skinning on the GPU.
*/

#version 420 core

const int MAX_JOINTS = 32;

uniform mat4 model;
uniform mat3 normal;
uniform mat4 viewproj;

uniform mat4 jointMatrices[MAX_JOINTS];

//Model-space vertex position.
layout(location = 0) in vec4 inPos;
//Model-space vertex normal.
layout(location = 1) in vec3 inNorm;
//Texture coordinates.
layout(location = 2) in vec2 inUV;
//Which joints affect the current vertex.
layout(location = 3) in vec4 inJoints;
//The weighting for each joint affecting the current vertex.
layout(location = 4) in vec4 inWeights;

layout(location = 0) out vec4 outPos;
layout(location = 1) out vec3 outNorm;
layout(location = 2) out vec2 outUV;

void main()
{
    mat4 skinMat = 
        inWeights.x * jointMatrices[int(inJoints.x)] +
        inWeights.y * jointMatrices[int(inJoints.y)] +
        inWeights.z * jointMatrices[int(inJoints.z)] +
        inWeights.w * jointMatrices[int(inJoints.w)];

    vec4 skinnedPos = skinMat * inPos;

    outPos = model * skinnedPos;
    outNorm = normalize(normal * (skinMat * vec4(inNorm, 0.0)).xyz);
    outUV = inUV;

    gl_Position = viewproj * outPos;
}