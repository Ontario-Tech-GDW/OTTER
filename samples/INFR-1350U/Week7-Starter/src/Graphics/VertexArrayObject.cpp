#include "VertexArrayObject.h"
#include "IndexBuffer.h"
#include "Logging.h"
#include "VertexBuffer.h"

VertexArrayObject::VertexArrayObject() :
	_indexBuffer(nullptr),
	_handle(0),
	_vertexCount(0)
{
	glCreateVertexArrays(1, &_handle);
}

VertexArrayObject::~VertexArrayObject()
{
	if (_handle != 0) {
		glDeleteVertexArrays(1, &_handle);
		_handle = 0;
	}
}

void VertexArrayObject::SetDebugName(const std::string& name) {
	glObjectLabel(GL_VERTEX_ARRAY, _handle, name.length(), name.c_str());
}

void VertexArrayObject::SetIndexBuffer(const IndexBuffer::sptr& ibo) {
	_indexBuffer = ibo;
	Bind();
	if (_indexBuffer != nullptr) _indexBuffer->Bind();
	else IndexBuffer::UnBind();
	UnBind();
}

void VertexArrayObject::AddVertexBuffer(const VertexBuffer::sptr& buffer, const std::vector<BufferAttribute>& attributes)
{
	if (_vertexCount == 0) {
		_vertexCount = buffer->GetElementCount();
	} else {
		LOG_ASSERT(buffer->GetElementCount() == _vertexCount, "All buffers bound to a VAO should be of the same size in our implementation!");
	}
	VertexBufferBinding binding;
	binding.Buffer = buffer;
	binding.Attributes = attributes;
	_vertexBuffers.push_back(binding);

	Bind();
	buffer->Bind();
	for (const BufferAttribute& attrib : attributes) {
		glEnableVertexArrayAttrib(_handle, attrib.Slot);
		glVertexAttribPointer(attrib.Slot, attrib.Size, attrib.Type, attrib.Normalized, attrib.Stride, (void*)attrib.Offset);
	}
	UnBind();

}

void VertexArrayObject::Bind() const {
	glBindVertexArray(_handle);
}

void VertexArrayObject::UnBind() {
	glBindVertexArray(0);
}

void VertexArrayObject::Render() const {
	Bind();
	if (_indexBuffer != nullptr) {
		glDrawElements(GL_TRIANGLES, _indexBuffer->GetElementCount(), _indexBuffer->GetElementType(), nullptr);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, _vertexCount / 3);
	}
	UnBind();
}
