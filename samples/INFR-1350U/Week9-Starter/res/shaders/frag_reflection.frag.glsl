#version 410

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

uniform samplerCube s_Environment;
uniform mat3 u_EnvironmentRotation;

uniform vec3  u_CamPos;

out vec4 frag_color;

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	frag_color = vec4(1, 0, 0, 1.0);
}