#include "Graphics/Texture2D.h"

int Texture2D::MAX_TEXTURE_SIZE = 0;

Texture2D::Texture2D(const Texture2DDescription& description) :
	_handle(0), _description(description)
{
	if (MAX_TEXTURE_SIZE == 0) {
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MAX_TEXTURE_SIZE);
		LOG_INFO("Maximum texture size on this renderer is {} texels", MAX_TEXTURE_SIZE);
	}

	_RecreateTexture();
}

Texture2D::~Texture2D() {
	if (glIsTexture(_handle)) {
		glDeleteTextures(1, &_handle);
	}
}

void Texture2D::_RecreateTexture() {
	// TODO: implement (see slides)
	if (_handle != 0) { 

	glDeleteTextures(1, &_handle);    
	_handle = 0; 

	}
	
	glCreateTextures(GL_TEXTURE_2D, 1, &_handle); 
	
	if (_description.Width * _description.Height > 0 && _description.Format != InternalFormat::Unknown) 
	{ 

	glTextureStorage2D(_handle, 1, *_description.Format, _description.Width, _description.Height);   
	glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, (GLenum)_description.HorizontalWrap);    
	glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, (GLenum)_description.VerticalWrap);   
	glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, (GLenum)_description.MinificationFilter);    
	glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, (GLenum)_description.MagnificationFilter); 

	}
}

void Texture2D::LoadData(const Texture2DData::sptr& data) {
	// TODO: implement (see slides)

	if (_description.Format == InternalFormat::Unknown) {
		_description.Format = data->GetRecommendedFormat();
	}
	if (_description.Width != data->GetWidth() ||
		_description.Height != data->GetHeight())
	{
		_description.Width = data->GetWidth();
		_description.Height = data->GetHeight();

		_RecreateTexture();
	}

	// Align the data store to the size of a single component in
	////See https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPixelStore.xhtml

	int componentSize = (GLint)GetTexelComponentSize(data->GetPixelType());
	glPixelStorei(GL_PACK_ALIGNMENT, componentSize);
	// Upload our data to our image
	glTextureSubImage2D(_handle, 0, 0, 0, _description.Width, _description.Height, *data->GetFormat(),
		*data->GetPixelType(), data->GetDataPtr());
	// We can get better error logs by attaching an object label!
	if (!data->DebugName.empty()) {
		glObjectLabel(GL_TEXTURE, _handle, data->DebugName.length(), data->DebugName.c_str());
	}


}

void Texture2D::Clear(const glm::vec4 color) {
	if (_handle != 0) {
		glClearTexImage(_handle, 0, GL_RGBA, GL_FLOAT, &color[0]);
	}
}

void Texture2D::Bind(int slot) {
	if (_handle != 0) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, _handle);
	}
}

void Texture2D::UnBind(int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
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
