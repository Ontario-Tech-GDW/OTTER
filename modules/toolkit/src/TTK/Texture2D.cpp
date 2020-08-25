#include "TTK/Texture2D.h"
#include "stb_image.h"

#include <iostream>
#include "Logging.h"

namespace TTK {
	Texture2D::Texture2D() :
		m_EdgeBehaviour(GL_CLAMP_TO_EDGE),
		m_Filtering(GL_LINEAR),
		m_Target(GL_TEXTURE_2D),
		m_TexWidth(0),
		m_TexHeight(0),
		m_TexID(0)
	{ }

	Texture2D::Texture2D(int _id, int _width, int _height, GLenum target) {
		m_TexID = _id;
		m_TexWidth = _width;
		m_TexHeight = _height;
		m_Target = target;
	}

	Texture2D::~Texture2D() {
		glDeleteTextures(1, &m_TexID);
	}

	void Texture2D::Bind(GLenum textureUnit /* = GL_TEXTURE0 */) {
		glActiveTexture(textureUnit);
		glBindTexture(m_Target, m_TexID);
	}

	void Texture2D::Unbind(GLenum textureUnit /* = GL_TEXTURE0 */)
	{
		glActiveTexture(textureUnit);
		glBindTexture(m_Target, 0);
	}

	void Texture2D::LoadTextureFromFile(const std::string& filePath)
	{
		int numChannels = 0;
		int width, height; 
		unsigned char* imageData = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0);

		LOG_ASSERT(imageData != nullptr, "Failed to load texture from \"{}\"", filePath);

		// We have data!
		GLenum internal_format, image_format;
		switch (numChannels)
		{
		case 2:
			internal_format = GL_RG8;
			image_format = GL_RG;
			break;
		case 3:
			internal_format = GL_RGB8;
			image_format = GL_RGB;
			break;

		case 4:
			internal_format = GL_RGBA8;
			image_format = GL_RGBA;
			break;

		default:
			std::cout << "Unsupported texture format. Texture may look strange." + filePath << std::endl;
			internal_format = GL_RGB8;
			image_format = GL_RGB;
			break;
		}

		if ((numChannels * width) % 4 != 0) {
			LOG_ERROR("Cannot load texture! The line size must be a multiple of 4! If your texture has less than 4 channels, ensure that the width of the texture * the number of channels is 4!");

		} else {
			CreateTexture(width, height, GL_TEXTURE_2D, GL_LINEAR, GL_CLAMP_TO_EDGE, internal_format, image_format, GL_UNSIGNED_BYTE, imageData);
		}
		
		// Free our image data, since it is already on the GPU
		stbi_image_free(imageData);
	}

	void Texture2D::CreateTexture(int w, int h, GLenum target, GLenum filtering, GLenum edgeBehaviour, GLenum internalFormat, GLenum textureFormat, GLenum dataType, void* data) {
		m_TexWidth = w;
		m_TexHeight = h;
		m_Filtering = filtering;
		m_EdgeBehaviour = edgeBehaviour;
		m_InternalFormat = internalFormat;
		m_TextureFormat = textureFormat;
		m_DataType = dataType;
		m_Target = target;

		GLenum error = 0;

		// Not necessary to enable GL_TEXTURE_* in modern context.
	//	glEnable(m_pTarget);
	//	error = glGetError();

		if (m_TexID)
			glDeleteTextures(1, &m_TexID);

		glGenTextures(1, &m_TexID);
		glBindTexture(target, m_TexID);
		error = glGetError();

		glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, filtering);
		glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, filtering);
		glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, edgeBehaviour);
		glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, edgeBehaviour);
		error = glGetError();

		glTexImage2D(m_Target, 0, internalFormat, w, h, 0, textureFormat, dataType, data);
		error = glGetError();

		if (error != 0)
			LOG_ERROR("An error has occured while creating a texture. Continuing...");

		glBindTexture(m_Target, 0);

	}

	void Texture2D::UpdateTexture(void* newDataPtr /*= nullptr*/)
	{
		if (newDataPtr == nullptr)
			return;

		glBindTexture(m_Target, m_TexID);

		if (newDataPtr != nullptr)
			glTexSubImage2D(m_Target, 0, 0, 0, m_TexWidth, m_TexHeight, m_TextureFormat, m_DataType, newDataPtr);

		glBindTexture(m_Target, 0);
	}
}
