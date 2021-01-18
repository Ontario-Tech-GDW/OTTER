#include "Texture2DData.h"

#include <filesystem>
#include <stb_image.h>

Texture2DData::Texture2DData(uint32_t width, uint32_t height, PixelFormat format, PixelType type, void* sourceData, InternalFormat recommendedFormat) :
	_width(width), _height(height), _format(format), _type(type), _data(nullptr), _recommendedFormat(recommendedFormat)
{
	LOG_ASSERT(width > 0 & height > 0, "Width and height must both be greater than zero! Got {}x{}", width, height);
	_dataSize = width * (size_t)height * GetTexelSize(_format, _type);
	_data = malloc(_dataSize);
	LOG_ASSERT(_data != nullptr, "Failed to allocate texture data!");
	if (sourceData != nullptr) {
		memcpy(_data, sourceData, _dataSize);
	}
}

Texture2DData::~Texture2DData() {
	free(_data);
}

Texture2DData::sptr Texture2DData::LoadFromFile(const std::string& file, bool forceRgba)
{
	// Variables that will store properties about our image
	int width, height, numChannels;
	const int targetChannels = forceRgba ? 4 : 0;

	// Use STBI to load the image
	stbi_set_flip_vertically_on_load(true);
	uint8_t* data = stbi_load(file.c_str(), &width, &height, &numChannels, targetChannels);

	// If we could not load any data, warn and return null
	if (data == nullptr) {
		LOG_WARN("STBI Failed to load image from \"{}\"", file); 
		return nullptr; 
	}

	// We should estimate a good format for our data

	// numChannels will store the number of channels in the image on disk, if we overrode that we should use the override value
	if (targetChannels != 0)
		numChannels = targetChannels;

	// We'll determine a recommended format for the image based on number of channels
	InternalFormat internal_format;
	PixelFormat    image_format;
	switch (numChannels) {
	case 1:
		internal_format = InternalFormat::R8;
		image_format = PixelFormat::Red;
		break;
	case 2:
		internal_format = InternalFormat::RG8;
		image_format = PixelFormat::RG;
		break;
	case 3:
		internal_format = InternalFormat::RGB8;
		image_format = PixelFormat::RGB;
		break;
	case 4:
		internal_format = InternalFormat::RGBA8;
		image_format = PixelFormat::RGBA;
		break;
	default:
		LOG_ASSERT(false, "Unsupported texture format for texture \"{}\" with {} channels", file, numChannels)
		break;
	}
	
	// This is one of those poorly documented things in OpenGL
	if ((numChannels * width) % 4 != 0) {
		LOG_WARN("The alignment of a horizontal line is not a multiple of 4, this will require a call to glPixelStorei(GL_PACK_ALIGNMENT)");
	}

	// Create the result and store our image data in it
	// Note that stbi will always give us an array of unsigned bytes (uint8_t)
	Texture2DData::sptr result = std::make_shared<Texture2DData>(width, height, image_format, PixelType::UByte, data, internal_format);
	result->DebugName = std::filesystem::path(file).filename().string();
	
	// We now have a copy in our ptr, we can free STBI's copy of it
	stbi_image_free(data);

	return result;
}
