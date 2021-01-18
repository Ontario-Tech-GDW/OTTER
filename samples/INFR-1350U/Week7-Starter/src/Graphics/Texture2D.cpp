#include "Texture2D.h"

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
}

void Texture2D::LoadData(const Texture2DData::sptr& data) {
	// TODO: implement (see slides)
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
