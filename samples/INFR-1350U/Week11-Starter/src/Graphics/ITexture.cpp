#include "ITexture.h"

#include "Logging.h"

ITexture::Limits ITexture::_limits = ITexture::Limits();
bool ITexture::_isStaticInit = false;

ITexture::ITexture()
	: _handle(0)
{
	if (!_isStaticInit) {
		// Example of reading limits from the OpenGL renderer
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_limits.MAX_TEXTURE_SIZE);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &_limits.MAX_TEXTURE_UNITS);
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &_limits.MAX_3D_TEXTURE_SIZE);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_limits.MAX_TEXTURE_IMAGE_UNITS);
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &_limits.MAX_ANISOTROPY);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		LOG_INFO("==== Texture Limits =====");
		LOG_INFO("\tSize:       {}", _limits.MAX_TEXTURE_SIZE);
		LOG_INFO("\tUnits:      {}", _limits.MAX_TEXTURE_UNITS);
		LOG_INFO("\t3D Size:    {}", _limits.MAX_3D_TEXTURE_SIZE);
		LOG_INFO("\tUnits (FS): {}", _limits.MAX_TEXTURE_IMAGE_UNITS);
		LOG_INFO("\tMax Aniso.: {}", _limits.MAX_ANISOTROPY);
		
		_isStaticInit = true;
	}
}

ITexture::~ITexture() {
	if (glIsTexture(_handle)) {
		glDeleteTextures(1, &_handle);
	}
}

void ITexture::Bind(int slot) const {
	if (_handle != 0) {
		//glActiveTexture(GL_TEXTURE0 + slot);
		glBindTextureUnit(slot, _handle);
	}
}

void ITexture::Unbind(int slot)
{
	//glActiveTexture(GL_TEXTURE0 + slot);
	glBindTextureUnit(slot, 0);
}


void ITexture::Clear(const glm::vec4 color) {
	if (_handle != 0) {
		glClearTexImage(_handle, 0, GL_RGBA, GL_FLOAT, &color[0]);
	}
}
