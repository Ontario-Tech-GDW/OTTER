#version 410
in vec3 color;
in vec2 texUV;

out vec4 frag_color;

uniform sampler2D myTextureSampler;

void main() {

    //LECTURE 10

	//This vector checks the color of the tetxture at a specific UV coordinate
	//If the color is red, then we discard that part of the texture from our mesh
	vec4 tex = texture(myTextureSampler, texUV);
	if(tex.r < 0.1)
		discard;

	frag_color = tex;

	//frag_color = texture(myTextureSampler, texUV);// * vec4(color, 1.0);
	
	
}