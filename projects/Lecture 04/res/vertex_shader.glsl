#version 410
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 1) out vec3 outColor;

//Lecture 04
uniform mat4 MVP;


void main() {
	// vertex position in clip space
	gl_Position = MVP * vec4(inPosition, 1.0);

	outColor = inColor;
}


	