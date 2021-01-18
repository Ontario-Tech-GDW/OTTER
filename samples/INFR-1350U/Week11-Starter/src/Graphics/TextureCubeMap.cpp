#include "TextureCubeMap.h"

TextureCubeMap::TextureCubeMap(const TextureCubeDesc& description) :
	ITexture(), _description(description)
{

	_RecreateTexture();
}

void TextureCubeMap::_RecreateTexture() {
	if (_handle != 0) {
		glDeleteTextures(1, &_handle);
		_handle = 0;
	}

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_handle);

	if (_description.Size > 0 && _description.Format != InternalFormat::Unknown)
	{
		glTextureStorage2D(_handle, 1, *_description.Format, _description.Size, _description.Size);

		glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, (GLenum)_description.MinificationFilter);
		glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, (GLenum)_description.MagnificationFilter);
	}
}

void TextureCubeMap::LoadData(const TextureCubeMapData::sptr& data) {
	if (_description.Size != data->GetSize())
	{
		_description.Size = data->GetSize();

		if (_description.Format == InternalFormat::Unknown) {
			_description.Format = data->GetRecommendedFormat();
		}

		_RecreateTexture();
	}

	// We can get better error logs by attaching an object label!
	if (!data->DebugName.empty()) {
		glObjectLabel(GL_TEXTURE, _handle, data->DebugName.length(), data->DebugName.c_str());
	}

	// Align the data store to the size of a single component in
	// See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPixelStore.xhtml
	int componentSize = (GLint)GetTexelComponentSize(data->GetPixelType());
	glPixelStorei(GL_PACK_ALIGNMENT, componentSize);

	// Upload our data to our image
	glTextureSubImage3D(_handle, 0, 0, 0, 0, _description.Size, _description.Size, 6, *data->GetFormat(), *data->GetPixelType(), data->GetDataPtr());

	if (_description.GenerateMipMaps) {
		glGenerateTextureMipmap(_handle);
	}
}

TextureCubeMap::sptr TextureCubeMap::LoadFromImages(const std::string& path)
{
	TextureCubeMapData::sptr data = TextureCubeMapData::LoadFromImages(path);
	TextureCubeMap::sptr result = TextureCubeMap::Create();
	result->LoadData(data);
	return result;
}

void TextureCubeMap::SetMinFilter(MinFilter filter) {
	_description.MinificationFilter = filter;
	if (_handle != 0) {
		glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, (GLenum)_description.MinificationFilter);
	}
}

void TextureCubeMap::SetMagFilter(MagFilter filter) {
	_description.MagnificationFilter = filter;
	if (_handle != 0) {
		glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, (GLenum)_description.MagnificationFilter);
	}
}