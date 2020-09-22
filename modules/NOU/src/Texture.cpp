/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Texture.cpp
Simple wrapper for STB image loading into 2D textures.
*/

#include "NOU/Texture.h"

#include "stb_image.h"

namespace nou
{
	Texture2D::Texture2D(const std::string& filename, bool useNearest)
	{
		int channels;
		m_width = 0;
		m_height = 0;

		//If your textures are all upside down, you'd want to switch this to false.
		//The TLDR here is that many image file formats specify textures from the top
		//down, but OpenGL uses a bottom origin for vertical coordinates.
		//Yes, this is annoying. And it isn't the only place you can have this problem.
		//If a texture is mucked up, flipping the Y coordinate might be a good first 
		//troubleshooting step.
		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(filename.c_str(),
										&m_width, &m_height, &channels, STBI_rgb_alpha);

		//Generate a new OpenGL texture.
		glGenTextures(1, &m_id);
		//Bind the texture to specify we want to change its properties/data.
		glBindTexture(GL_TEXTURE_2D, m_id);

		//Sets our texture to repeat if accessed outside the (0, 1) texture
		//coordinate interval.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//Sets up a linear (smooth) filter for interpolating our texture
		//when displaying it smaller or larger (e.g., on a faraway or close-up object).
		if (useNearest)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		//Specifies our image data as the source data for the new texture.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//Very important - after we send our data to OpenGL, make sure to free the memory
		//used by STBI!
		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_id);
	}

	GLuint Texture2D::GetID() const
	{
		return m_id;
	}

	void Texture2D::GetDimensions(int& width, int& height) const
	{
		width = m_width;
		height = m_height;
	}
}