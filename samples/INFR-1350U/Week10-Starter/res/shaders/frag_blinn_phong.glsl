#version 410

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

uniform vec3  u_AmbientCol;
uniform float u_AmbientStrength;

uniform vec3  u_LightPos;
uniform vec3  u_LightCol;
uniform float u_AmbientLightStrength;
uniform float u_SpecularLightStrength;
uniform float u_Shininess;

uniform vec3  u_CamPos;

out vec4 frag_color;

// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
void main() {
	// Lecture 5
	vec3 ambient = ((u_AmbientLightStrength * u_LightCol) + (u_AmbientCol * u_AmbientStrength));

	// Diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(u_LightPos - inPos);

	float dif = max(dot(N, lightDir), 0.0);
	vec3 diffuse = dif * u_LightCol;// add diffuse intensity

	//Attenuation
	float dist = length(u_LightPos - inPos);
	diffuse = diffuse / dist; // (dist*dist)

	// Specular
	vec3 viewDir  = normalize(u_CamPos - inPos);
	vec3 h        = normalize(lightDir + viewDir);

	//vec3 camPos = u_CamPos;//Pass this as a uniform from your C++ code
	//float specularStrength = 1.0; // this can be a uniform
	//vec3 camDir = normalize(camPos - inPos);
	//vec3 reflectDir = reflect(-lightDir, N);
	// New
	float spec = pow(max(dot(N, h), 0.0), u_Shininess); // Shininess coefficient (can be a uniform)
	vec3 specular = u_SpecularLightStrength * spec * u_LightCol; // Can also use a specular color

	vec3 result = (ambient + diffuse + specular) * inColor;

	frag_color = vec4(result, 1.0);
}