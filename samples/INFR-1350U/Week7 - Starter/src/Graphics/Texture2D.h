#pragma once
#include <memory>
#include <cstdint>
#include <GLM/glm.hpp>

#include "TextureEnums.h"
#include "Texture2DData.h"

struct Texture2DDescription
{
	uint32_t       Width;
	uint32_t       Height;
	InternalFormat Format;
	WrapMode       HorizontalWrap;
	WrapMode       VerticalWrap;
	MinFilter      MinificationFilter;
	MagFilter      MagnificationFilter;

	Texture2DDescription() :
		Width(0), Height(0),
		Format(InternalFormat::Unknown),
		HorizontalWrap(WrapMode::Repeat),
		VerticalWrap(WrapMode::Repeat),
		MinificationFilter(MinFilter::NearestMipLinear),
		MagnificationFilter(MagFilter::Linear)
	{ }
};

/// <summary>
/// Represents a wrapper around a 2D OpenGL texture
/// </summary>
class Texture2D final
{
public:
	// We'll disallow moving and copying, since we want to manually control when the destructor is called
	// We'll use these classes via pointers
	Texture2D(const Texture2D& other) = delete;
	Texture2D(Texture2D&& other) = delete;
	Texture2D& operator=(const Texture2D& other) = delete;
	Texture2D& operator=(Texture2D&& other) = delete;

	typedef std::shared_ptr<Texture2D> sptr;
	static inline sptr Create(const Texture2DDescription& description = Texture2DDescription()) {
		return std::make_shared<Texture2D>(description);
	}

public:
	/// <summary>
	/// Creates a new texture with the given description
	/// </summary>
	/// <param name="description">The default description for the texture</param>
	Texture2D(const Texture2DDescription& description);
	~Texture2D();

	/// <summary>
	/// Uploads data to this texture
	/// </summary>
	/// <param name="data">The texture data to upload into this texture</param>
	void LoadData(const Texture2DData::sptr& data);
	/// <summary>
	/// Clears this texture to a given color
	/// </summary>
	/// <param name="color">The color to clear the texture to</param>
	void Clear(const glm::vec4 color = glm::vec4(1.0f));

	/// <summary>
	/// Binds this texture to the given texture slot
	/// </summary>
	/// <param name="slot">The slot to bind the texture to</param>
	void Bind(int slot);
	/// <summary>
	/// Unbinds a texture from the given slot
	/// </summary>
	/// <param name="slot">The slot to unbind a texture from</param>
	static void UnBind(int slot);

	/// <summary>
	/// Gets the underlying OpenGL handle for this texture
	/// </summary>
	GLuint GetHandle() const { return _handle; }
	
	uint32_t GetWidth() const { return _description.Width; }
	uint32_t GetHeight() const { return _description.Height; }
	InternalFormat GetFormat() const { return _description.Format; }	
	MinFilter GetMinFilter() const { return _description.MinificationFilter; }
	MagFilter GetMagFilter() const { return _description.MagnificationFilter; }
	WrapMode GetWrapS() const { return _description.HorizontalWrap; }
	WrapMode GetWrapT() const { return _description.VerticalWrap; }
	
	void SetMinFilter(MinFilter filter);
	void SetMagFilter(MagFilter filter);
	void SetWrapS(WrapMode mode);
	void SetWrapT(WrapMode mode);

	const Texture2DDescription& GetDescription() const { return _description; }
	
private:
	Texture2DDescription _description;
	GLuint _handle;

	void _RecreateTexture();
	
	static int MAX_TEXTURE_SIZE;
};
