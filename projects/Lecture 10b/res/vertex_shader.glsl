#version 410
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_uv;

out vec3 color;
out vec2 texUV;

uniform mat4 MVP;

uniform sampler2D myTextureSampler;

//LECTURE 10b
uniform float time;


void main() {

	
	color = vertex_color;
	texUV = vertex_uv;

	//LECTURE 10b
	vec3 vert = vertex_pos;
	//vert.y = texture(myTextureSampler, vertex_uv).r;
	
	vert.y = sin(vert.x * 5.0 + time *0.1) *0.2;

	gl_Position = MVP * vec4(vert, 1.0);

	//gl_Position = MVP * vec4(vertex_pos, 1.0);
}
	