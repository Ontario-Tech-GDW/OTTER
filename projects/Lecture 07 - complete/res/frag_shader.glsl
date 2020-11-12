#version 410

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
//LECTURE 7
layout(location = 3) in vec2 UV;

uniform sampler2D textureSampler;
uniform sampler2D textureSampler2;

uniform vec3 LightPos;


out vec4 frag_color;


void main() {
	
	vec3 textureColor = texture(textureSampler, UV).xyz;


	// Lecture 5
	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor * textureColor;//inColor;

	// Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(LightPos - inPos);
	
	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = dif * textureColor;//inColor;// add diffuse intensity

	//Attenuation
	float dist = length(LightPos - inPos);
	diffuse = diffuse / dist; // (dist*dist)
	
	// Specular
	vec3 camPos = vec3(0.0, 0.0, 3.0) ;//Pass this as a uniform from your C++ code
	float specularStrength = 1.0; // this can be a uniform
	vec3 camDir = normalize(camPos - inPos);
	vec3 reflectDir = reflect(-lightDir, N);
	float spec = pow(max(dot(camDir, reflectDir), 0.0), 4); // Shininess coefficient (can be a uniform)
	vec3 specular = specularStrength * spec * lightColor; // Can also use a specular color

	vec3 result = (ambient + diffuse + specular);

	frag_color = texture(textureSampler, UV) * vec4(result, 1.0);
	//frag_color = texture(textureSampler2, UV) * vec4(result, 1.0);
}