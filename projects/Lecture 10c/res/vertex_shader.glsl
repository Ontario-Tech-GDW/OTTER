#version 410
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 vertex_uv;

uniform mat4 Model, View, Projection;
out vec3 color;
out vec2 texUV;
out vec3 world_normal;
out vec3 world_pos;

//uniform mat4 MVP;

void main() {

	world_normal = normalize(mat3(Model) * vertex_normal);
	world_pos = mat3(Model) * vertex_pos;
	color = vertex_color;

	gl_Position = Projection * View * Model * vec4(vertex_pos, 1.0);
	texUV = vertex_uv;
}
	