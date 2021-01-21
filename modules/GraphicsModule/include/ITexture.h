#pragma once
#include <cstdint>
#include <memory>
#include <glad/glad.h>
#include <GLM/glm.hpp>

class ITexture
{
public:
	typedef std::shared_ptr<ITexture> sptr;
	typedef std::unique_ptr<ITexture> uptr;

	/// <summary>
	/// Represents the limits available to OpenGL textures on the current renderer
	/// </summary>
	struct Limits {
		int   MAX_TEXTURE_SIZE;
		int   MAX_TEXTURE_UNITS;
		int   MAX_3D_TEXTURE_SIZE;
		int   MAX_TEXTURE_IMAGE_UNITS;
		float MAX_ANISOTROPY;
	};

	/// <summary>
	/// Gets the texture limits of the current GPU
	/// </summary>
	/// <returns>A structure containing all the texture limits of the GPU</returns>
	static const Limits& GetLimits() { return _limits; }
	
	/// <summary>
	/// Unbinds a texture from the given slot
	/// </summary>
	/// <param name="slot">The slot to unbind a texture from</param>
	static void Unbind(int slot);

	/// <summary>
	/// Gets the underlying OpenGL handle for this texture
	/// </summary>
	GLuint GetHandle() const { return _handle; }
	
	/// <summary>
	/// Clears this texture to a given color
	/// </summary>
	/// <param name="color">The color to clear the texture to</param>
	void Clear(const glm::vec4 color = glm::vec4(1.0f));

	/// <summary>
	/// Binds this texture to the given texture slot
	/// </summary>
	/// <param name="slot">The slot to bind the texture to</param>
	void Bind(int slot) const;
	
protected:
	ITexture();
	virtual ~ITexture();

	GLuint _handle;

	static Limits _limits;
	static bool _isStaticInit;
};
