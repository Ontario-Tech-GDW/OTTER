#include "Texture2D.h"

Texture2D::Texture2D(const Texture2DDescription& description) :
	ITexture(), _description(description)
{

	_RecreateTexture();
}

void Texture2D::_RecreateTexture() {
	if (_handle != 0) {
		glDeleteTextures(1, &_handle);
		_handle = 0;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &_handle);

	if (_description.MaxAnisotropic < 0.0f) {
		_description.MaxAnisotropic = ITexture::GetLimits().MAX_ANISOTROPY;
	}

	if (_description.Width * _description.Height > 0 && _description.Format != InternalFormat::Unknown)
	{
		glTextureStorage2D(_handle, 1, *_description.Format, _description.Width, _description.Height);

		glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, (GLenum)_description.HorizontalWrap);
		glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, (GLenum)_description.VerticalWrap);
		glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, (GLenum)_description.MinificationFilter);
		glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, (GLenum)_description.MagnificationFilter);
		glTextureParameterf(_handle, GL_TEXTURE_MAX_ANISOTROPY, _description.MaxAnisotropic);
	}
}

void Texture2D::LoadData(const Texture2DData::sptr& data) {
	if (_description.Width != data->GetWidth() ||
		_description.Height != data->GetHeight()) 
	{
		_description.Width = data->GetWidth();
		_description.Height = data->GetHeight();
		
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
	glTextureSubImage2D(_handle, 0, 0, 0, _description.Width, _description.Height, *data->GetFormat(), *data->GetPixelType(), data->GetDataPtr());

	if (_description.GenerateMipMaps) {
		glGenerateTextureMipmap(_handle);
	}
}

Texture2D::sptr Texture2D::LoadFromFile(const std::string& path) {
	Texture2DData::sptr data = Texture2DData::LoadFromFile(path);
	LOG_ASSERT(data != nullptr, "Failed to load image from file!");
	Texture2D::sptr result = Texture2D::Create();
	result->LoadData(data);
	return result;
}

void Texture2D::SetMinFilter(MinFilter filter) {
	_description.MinificationFilter = filter;
	if (_handle != 0) {
		glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, (GLenum)_description.MinificationFilter);
	}
}

void Texture2D::SetMagFilter(MagFilter filter) {
	_description.MagnificationFilter = filter;
	if (_handle != 0) {
		glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, (GLenum)_description.MagnificationFilter);
	}
}

void Texture2D::SetWrapS(WrapMode mode) {
	_description.HorizontalWrap = mode;
	if (_handle != 0) {
		glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, (GLenum)_description.HorizontalWrap);
	}
}

void Texture2D::SetWrapT(WrapMode mode) {
	_description.VerticalWrap = mode;
	if (_handle != 0) {
		glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, (GLenum)_description.VerticalWrap);
	}
}

void Texture2D::SetAnisotropicFiltering(float level)
{
	if (level < 0.0f) {
		level = ITexture::GetLimits().MAX_ANISOTROPY;
	}
	_description.MaxAnisotropic = level;
	if (_handle != 0) {
		glTextureParameterf(_handle, GL_TEXTURE_MAX_ANISOTROPY, _description.MaxAnisotropic);
	}
}
