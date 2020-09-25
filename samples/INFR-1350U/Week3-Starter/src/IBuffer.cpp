#include "IBuffer.h"

IBuffer::IBuffer(GLenum type, GLenum usage) :
	_elementCount(0),
	_elementSize(0),
	_handle(0)
{
	// TODO: implement
}

IBuffer::~IBuffer() {
	// TODO: implement
}

void IBuffer::LoadData(const void* data, size_t elementSize, size_t elementCount) {
	// TODO: implement
}

void IBuffer::Bind() {
	glBindBuffer(_type, _handle);
}

void IBuffer::UnBind(GLenum type) {
	glBindBuffer(type, 0);
}
