#pragma once
#include <vector>
#include <Texture2D.h>
#include <Shader.h>

struct DepthTarget
{
	//Deconstructor for Depth Target
	//*Unloads texture
	~DepthTarget();
};

struct ColorTarget
{
	//Deconstructor for Color Target
	//*Unloads all the color targets
	~ColorTarget();
};

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

protected:

};