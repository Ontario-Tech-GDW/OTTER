#pragma once
#include "IBuffer.h"
#include <memory>

/// <summary>
/// The vertex buffer will store all of our vertex data for rendering
/// </summary>
class VertexBuffer : public IBuffer
{
public:
	typedef std::shared_ptr<VertexBuffer> Sptr;

	static inline Sptr Create(BufferUsage usage = BufferUsage::StaticDraw) {
		return std::make_shared<VertexBuffer>(usage);
	}

	/// <summary>
	/// Creates a new vertex buffer, with the given usage. Data will still need to be uploaded before it can be used
	/// </summary>
	/// <param name="usage">The usage hint for the buffer, default is GL_STATIC_DRAW</param>
	VertexBuffer(BufferUsage usage = BufferUsage::StaticDraw) : IBuffer(BufferType::Vertex, usage) { }
	
	/// <summary>
	/// Unbinds the current vertex buffer
	/// </summary>
	static void UnBind() { IBuffer::UnBind(BufferType::Vertex); }
};

