#pragma once
#include <memory>
#include <cstdint>

#include "TextureEnums.h"

/// <summary>
/// Stores data required to upload texture data into OpenGL
/// </summary>
class Texture2DData final
{
public:
	Texture2DData(const Texture2DData& other) = delete;
	Texture2DData(Texture2DData&& other) = delete;
	Texture2DData& operator=(const Texture2DData& other) = delete;
	Texture2DData& operator=(Texture2DData&& other) = delete;
	typedef std::shared_ptr<Texture2DData> sptr;

	std::string DebugName;

	/// <summary>
	/// Creates a new 2D texture data object
	/// </summary>
	/// <param name="width">The width of the texture, in pixels</param>
	/// <param name="height">The height of the texture, in pixels</param>
	/// <param name="format">The pixel format or layout of a pixel (ex: RGBA)</param>
	/// <param name="type">The component type of the pixel (ex: uint8_t)</param>
	/// <param name="sourceData">A pointer to the data to upload to this texture</param>
	/// <param name="recommendedFormat">The recommended internal format to use when creating textures from this data</param>
	Texture2DData(uint32_t width, uint32_t height, PixelFormat format, PixelType type, void* sourceData, InternalFormat recommendedFormat = InternalFormat::Unknown);
	~Texture2DData();

	/// <summary>
	/// Loads image data from an external file
	/// </summary>
	/// <param name="file">The path of the file to load</param>
	/// <param name="forceRgba">True to force STBI to load 4 component texture data</param>
	/// <returns>A pointer to the data loaded from the file, or nullptr if the file failed to load</returns>
	static Texture2DData::sptr LoadFromFile(const std::string& file, bool forceRgba = false);

	/// <summary>
	/// Gets the width of the texture data, in pixels
	/// </summary>
	uint32_t GetWidth() const { return _width; }
	/// <summary>
	/// Gets the height of the texture data, in pixels
	/// </summary>
	uint32_t GetHeight() const { return _height; }
	/// <summary>
	/// Gets the Pixel Format (RG, RGB, RGBA, etc) of the texture data
	/// </summary>
	PixelFormat GetFormat() const { return _format; }
	/// <summary>
	/// Gets the underlying data type of a single component
	/// </summary>
	PixelType  GetPixelType() const { return _type; }
	/// <summary>
	/// Gets a recommended internal format to use when creating a texture using this data, may be
	/// InternalFormat::Unknown
	/// </summary>
	InternalFormat  GetRecommendedFormat() const { return _recommendedFormat; }
	/// <summary>
	/// Get the total size of the underlying data (size of individual pixel * width * height)
	/// </summary>
	size_t  GetDataSize() const { return _dataSize; }
	/// <summary>
	/// Gets a readonly copy of the underlying data in this image for upload
	/// </summary>
	const void* GetDataPtr() const { return _data; }

private:
	uint32_t    _width, _height;
	size_t      _dataSize;
	PixelFormat _format;
	PixelType   _type;
	InternalFormat _recommendedFormat;
	void* _data;
};