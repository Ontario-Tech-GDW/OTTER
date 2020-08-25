//////////////////////////////////////////////////////////////////////////
//
// This header is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this header in your GDW games.
// 
// This is a simple OpenGL Texture2D class
//
// Michael Gharbharan 2015 - 2017
// Shawn Matthews - 2019
//
//////////////////////////////////////////////////////////////////////////

#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <string>
#include "glad/glad.h"
#include <memory>

namespace TTK {
	class  Texture2D
	{
	public:
		typedef std::shared_ptr<Texture2D> Ptr;

		/*
		 * Creates a blank texture object. Load data into it with
		 * CreateTexture and LoadTextureFromFile
		 */
		Texture2D();
		/*
		 * Creates a new texture without any pixel data. Load data into it with
		 * CreateTexture and LoadTextureFromFile
		 */
		Texture2D(int _id, int _width, int _height, GLenum target);
		/*
		 * Destroys a texture and cleans up it's underlying data on the GPU
		 */
		~Texture2D();

		/*
		 * Gets the width of this texture, in pixels
		 */
		int GetWidth() const { return m_TexWidth; };
		/*
		 * Gets the height of this texture, in pixels
		 */
		int GetHeight() const { return m_TexHeight; };

		/*
		 * Gets the internal format of this texture
		 * @returns The internal format of the texture
		 */
		GLenum GetInternalFormat() const { return m_InternalFormat; }

		/*
		 * Binds this texture to a given texture slot, should be one of GL_TEXTUREx
		 * @param textureUnit The texture unit to bind this texture to
		 */
		void Bind(GLenum textureUnit = GL_TEXTURE0);
		/*
		 * This texture from a given slot, slot should be one of GL_TEXTUREx
		 * @param textureUnit The texture unit to unbind
		 */
		void Unbind(GLenum textureUnit = GL_TEXTURE0);

		/*
		 * Loads a texture file from the given file path into this texture instance
		 * @param filePath The path to the file relative to the current working directory
		 */
		void LoadTextureFromFile(const std::string& filePath);

		/*
		 * Creates the texture, allocates memory and uploads data to GPU
		 * If you do not want to upload data to the GPU pass in a nullptr for the dataPtr.
		 * For a description on filtering and edgeBehaviour see https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
		 * For a description on internalFormat, textureFormat and dataType see https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
		 *
		 * @param w The width of the texture, in pixels
		 * @param h The height of the texture, in pixels
		 * @param target The target to save this texture to, should be GL_TEXTURE_2D, or can also be GL_TEXTURE_1D_ARRAY
		 * @param edgeBehaviour The texture wrapping mode to use when accessing texels beyond the edge of the texture
		 * @param internalFormat The internal format of the texture, usually GL_RGB8 or GL_RGBA8
		 * @param textureFormat The format of the texture data that we are writing to the texture, usually GL_RGB or GL_RGBA
		 * @param dataType The type of components that we are writing to the texture, usually GL_UNSIGNED_BYTE
		 * @param data A pointer to the data to write to the texture
		 */
		void CreateTexture(int w, int h, GLenum target, GLenum filtering, GLenum edgeBehaviour, GLenum internalFormat, GLenum textureFormat, GLenum dataType, void* data);

		/*
		 * Uploads new data to a texture, using it's saved internal format and data types
		 * @param newDataPtr The new data to write to the texture
		 */
		void UpdateTexture(void* newDataPtr = nullptr);

		/*
		 * Returns the underlying OpenGL texture ID for this texture
		 */
		unsigned int GetID() const { return m_TexID; };

	private:
		unsigned int m_TexWidth;
		unsigned int m_TexHeight;
		unsigned int m_TexID;

		GLenum m_Filtering;
		GLenum m_EdgeBehaviour;
		GLenum m_InternalFormat;
		GLenum m_TextureFormat;
		GLenum m_DataType;

		GLenum m_Target; // usually GL_TEXTURE_2D
	};
}

#endif