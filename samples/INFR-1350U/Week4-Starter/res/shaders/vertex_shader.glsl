#version 410
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

uniform mat4 u_ModelViewProjection;

layout(location = 1) out vec3 outColor;

void main() {
	// vertex position in clip space
	gl_Position = u_ModelViewProjection * vec4(inPosition, 1.0);

	outColor = inColor;
}


