#include "TextureCubeMap.h"

TextureCubeMap::TextureCubeMap(const TextureCubeDesc& description) :
	ITexture(), _description(description) {
	_RecreateTexture();
}

void TextureCubeMap::_RecreateTexture() {

}

void TextureCubeMap::LoadData(const TextureCubeMapData::sptr& data) {

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