#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

VertexArrayObject::VertexArrayObject() :
	_indexBuffer(nullptr),
	_handle(0)
{
	glCreateVertexArrays(1, &_handle);
}

VertexArrayObject::~VertexArrayObject()
{
	if (_handle != 0) 
	{
		glDeleteVertexArrays(1, &_handle); 
		_handle = 0; 
	}
}

void VertexArrayObject::SetIndexBuffer(IndexBuffer* ibo) {
	_indexBuffer = ibo;   
	Bind();
		if (_indexBuffer != nullptr)_indexBuffer->Bind(); 
		else IndexBuffer::Unbind();   
	Unbind();
}

void VertexArrayObject::AddVertexBuffer(VertexBuffer* buffer, const std::vector<BufferAttribute>& attributes)
{
	VertexBufferBinding binding;   
	binding.Buffer = buffer;    
	binding.Attributes = attributes;   
	_vertexBuffers.push_back(binding);

	Bind();   
	buffer->Bind(); 
	for (const BufferAttribute& attrib : attributes) 
	{ 
		glEnableVertexArrayAttrib(_handle, attrib.Slot); 
		glVertexAttribPointer(attrib.Slot, attrib.Size, (GLenum)attrib.Type, attrib.Normalized, attrib.Stride, (void*)attrib.Offset); 
	}    
	Unbind();
}

void VertexArrayObject::Bind() {
	glBindVertexArray(_handle);
}

void VertexArrayObject::Unbind() {
	glBindVertexArray(0);
}
