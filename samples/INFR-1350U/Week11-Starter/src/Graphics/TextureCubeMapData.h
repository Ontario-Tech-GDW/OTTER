#pragma once
#include <memory>
#include <string>

#include "TextureEnums.h"

#include "Texture2DData.h"

/*
0 	GL_TEXTURE_CUBE_MAP_POSITIVE_X
1 	GL_TEXTURE_CUBE_MAP_NEGATIVE_X
2 	GL_TEXTURE_CUBE_MAP_POSITIVE_Y
3 	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
4 	GL_TEXTURE_CUBE_MAP_POSITIVE_Z
5 	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
 */
ENUM(CubeMapFace, GLint,
	PosX = 0,
	NegX = 1,
	PosY = 2,
	NegY = 3,
	PosZ = 4,
	NegZ = 5
);

/// <summary>
/// Stores data required to upload texture data into OpenGL
/// </summary>
class TextureCubeMapData final
{
public:
	TextureCubeMapData(const TextureCubeMapData& other) = delete;
	TextureCubeMapData(TextureCubeMapData&& other) = delete;
	TextureCubeMapData& operator=(const TextureCubeMapData& other) = delete;
	TextureCubeMapData& operator=(TextureCubeMapData&& other) = delete;
	typedef std::shared_ptr<TextureCubeMapData> sptr;

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
	TextureCubeMapData(uint32_t size, PixelFormat format, PixelType type, void* sourceData, InternalFormat recommendedFormat = InternalFormat::Unknown);
	~TextureCubeMapData();

	/// <summary>
	/// Loads a cubemap from a set of 6 images
	/// </summary>
	/// <param name="images">The set of images to load the cubemap from, see CubeMapFace for the ordering</param>
	/// <returns>A pointer to the data created from the images</returns>
	static TextureCubeMapData::sptr CreateFromImages(const std::vector<Texture2DData::sptr>& images);

	/// <summary>
	/// Loads a cubemap from a set of 6 images stored in different files. The files should all follow a common naming scheme, and
	/// be the same file type (ex: PNG). The naming will be as follows:
	/// image_neg_x.png --> CubeMapFace::NegX
	/// image_pos_x.png --> CubeMapFace::PosX
	/// image_neg_y.png --> CubeMapFace::NegY
	/// image_pos_y.png --> CubeMapFace::PosY
	/// image_neg_z.png --> CubeMapFace::NegZ
	/// image_pos_z.png --> CubeMapFace::PosZ
	/// </summary>
	/// <param name="rootImagePath">The base path for images, including extension. This file name will be appended with _pos_x, _neg_x, etc...</param>
	/// <returns>A pointer to the data created from the images</returns>
	static TextureCubeMapData::sptr LoadFromImages(const std::string& rootImagePath);

	/// <summary>
	/// Loads 2D image data into this cubemap data for the given face. Dimensions and format must match the existing size and formats
	/// </summary>
	/// <param name="data">The data to load into the face</param>
	/// <param name="face">The face to load data into</param>
	void LoadFaceData(const Texture2DData::sptr& data, CubeMapFace face);

	/// <summary>
	/// Gets the size of the texture (width/height of each individual image in the set)
	/// </summary>
	uint32_t GetSize() const { return _size; }
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
	/// Get the total size of the underlying data (size of individual pixel * width * height * 6)
	/// </summary>
	size_t  GetDataSize() const { return _dataSize; }
	/// <summary>
	/// Returns the size of a single face worth's of data, in bytes
	/// </summary>
	/// <returns></returns>
	size_t GetFaceDataSize() const { return _faceDataSize; }
	/// <summary>
	/// Gets a readonly copy of the underlying data in this image for upload
	/// </summary>
	const void* GetDataPtr() const { return _data; }

	/// <summary>
	/// Gets a readonly copy of the data for a single face in this cube map
	/// </summary>
	/// <param name="face">The face to get the data for</param>
	/// <returns>A const pointer to the start of data for the given face</returns>
	const void* GetFaceDataPtr(CubeMapFace face) const { return static_cast<char*>(_data) + (_faceDataSize * (size_t)face); }

private:
	uint32_t    _size;
	size_t      _dataSize;
	size_t      _faceDataSize;
	PixelFormat _format;
	PixelType   _type;
	InternalFormat _recommendedFormat;
	void* _data;
};