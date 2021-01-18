#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outColor;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec2 outUV;
layout(location = 4) out vec4 outTexWeights;

uniform mat4 u_ModelViewProjection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat3 u_NormalMatrix;
uniform vec3 u_LightPos;

uniform vec3  u_HeightCutoffs;
uniform vec3  u_InterpolateFactors;

float GetTextureWeight(float height, float a, float b) {
	float t = min(max((height-a) / (b-a), 0), 1);
	return t * t * (3 - 2 * t);
}

void main() {

	gl_Position = u_ModelViewProjection * vec4(inPosition, 1.0);

	// Lecture 5
	// Pass vertex pos in world space to frag shader
	outPos = (u_Model * vec4(inPosition, 1.0)).xyz;

	// Normals
	outNormal = u_NormalMatrix * inNormal;

    // Do heightmap stuff here

	// Pass our UV coords to the fragment shader
	outUV = inUV * 128.0;

    
    float height = inUV.x;

    vec3 upperEdge = u_HeightCutoffs + u_InterpolateFactors;
    vec3 lowerEdge = u_HeightCutoffs - u_InterpolateFactors;

	outTexWeights = vec4(
		GetTextureWeight(height, upperEdge.x, lowerEdge.x),
		min(GetTextureWeight(height, lowerEdge.x, upperEdge.x), GetTextureWeight(height, upperEdge.y, lowerEdge.y)),
		min(GetTextureWeight(height, lowerEdge.y, upperEdge.y), GetTextureWeight(height, upperEdge.z, lowerEdge.z)),
		GetTextureWeight(height, lowerEdge.z, upperEdge.z)
	);

	///////////
	outColor = inColor;

}

