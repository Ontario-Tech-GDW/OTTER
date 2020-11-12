#version 410
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_uv;

out vec3 outcolor;
out vec2 texUV;

uniform mat4 MVP;

void main() {
	outcolor = vertex_color;
	texUV = vertex_uv;

	//LECTURE 10

	vec3 vert = vertex_pos;
	vert.y = 0.0;//flatten the object
	gl_Position = MVP * vec4(vert, 1.0);

	//gl_Position = MVP * vec4(vertex_pos, 1.0);
	
}
	