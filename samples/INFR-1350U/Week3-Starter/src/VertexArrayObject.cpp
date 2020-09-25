#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

VertexArrayObject::VertexArrayObject() :
	_indexBuffer(nullptr),
	_handle(0)
{
	// TODO: implement
}

VertexArrayObject::~VertexArrayObject()
{
	// TODO: implement
}

void VertexArrayObject::SetIndexBuffer(IndexBuffer* ibo) {
	// TODO: implement
}

void VertexArrayObject::AddVertexBuffer(VertexBuffer* buffer, const std::vector<BufferAttribute>& attributes)
{
	// TODO: implement
}

void VertexArrayObject::Bind() {
	glBindVertexArray(_handle);
}

void VertexArrayObject::UnBind() {
	glBindVertexArray(0);
}
