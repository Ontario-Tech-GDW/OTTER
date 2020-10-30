#pragma once
#include <glad/glad.h>
#include <cstdint>
#include <vector>

// We can declare the classes for IndexBuffer and VertexBuffer here, since we don't need their full definitions in the .h file

class IndexBuffer;
class VertexBuffer;

/// <summary>
/// This structure will represent the parameters passed to the glVertexAttribPointer commands
/// </summary>
struct BufferAttribute
{
	/// <summary>
	/// The input slot to the vertex shader that will receive the data
	/// </summary>
	GLuint  Slot;
	/// <summary>
	/// The number of elements to be passed (ex 3 for a vec3)
	/// </summary>
	GLint   Size;
	/// <summary>
	/// The type of data to be passed (ex: GL_FLOAT for a vec3)
	/// </summary>
	GLenum  Type;
	/// <summary>
	/// Whether or not the data should be normalized into the 0-1 range (usually this is false)
	/// </summary>
	bool    Normalized;
	/// <summary>
	/// The total size of an element in this buffer
	/// </summary>
	GLsizei Stride;
	/// <summary>
	/// The offset from the start of an element to this attribute
	/// </summary>
	GLsizei Offset;

	BufferAttribute(uint32_t slot, uint32_t size, GLenum type, bool normalized, GLsizei stride, GLsizei offset) :
		Slot(slot), Size(size), Type(type), Normalized(normalized), Stride(stride), Offset(offset) { }
};

/// <summary>
/// The Vertex Array Object wraps around an OpenGL VAO and basically represents all of the data for a mesh
/// </summary>
class VertexArrayObject final
{
public:
	// We'll disallow moving and copying, since we want to manually control when the destructor is called
	// We'll use these classes via pointers
	VertexArrayObject(const VertexArrayObject& other) = delete;
	VertexArrayObject(VertexArrayObject&& other) = delete;
	VertexArrayObject& operator=(const VertexArrayObject& other) = delete;
	VertexArrayObject& operator=(VertexArrayObject&& other) = delete;
	
public:
	/// <summary>
	/// Creates a new empty Vertex Array Object
	/// </summary>
	VertexArrayObject();
	// Destructor does not need to be virtual due to the use of the final keyword
	~VertexArrayObject();

	/// <summary>
	/// Sets the index buffer for this VAO, note that for now, this will not delete the buffer when the VAO is deleted, more on that later
	/// </summary>
	/// <param name="ibo">The index buffer to bind to this VAO</param>
	void SetIndexBuffer(IndexBuffer* ibo);
	/// <summary>
	/// Adds a vertex buffer to this VAO, with the specified attributes
	/// </summary>
	/// <param name="buffer">The buffer to add (note, does not take ownership, you will still need to delete later)</param>
	/// <param name="attributes">A list of vertex attributes that will be fed by this buffer</param>
	void AddVertexBuffer(VertexBuffer* buffer, const std::vector<BufferAttribute>& attributes);

	/// <summary>
	/// Binds this VAO as the source of data for draw operations
	/// </summary>
	void Bind();
	/// <summary>
	/// Unbinds the currently bound VAO
	/// </summary>
	static void UnBind();

	/// <summary>
	/// Returns the underlying OpenGL handle that this class is wrapping around
	/// </summary>
	GLuint GetHandle() const { return _handle; }
	
protected:
	// Helper structure to store a buffer and the attributes
	struct VertexBufferBinding
	{
		VertexBuffer* Buffer;
		std::vector<BufferAttribute> Attributes;
	};
	
	// The index buffer bound to this VAO
	IndexBuffer* _indexBuffer;
	// The vertex buffers bound to this VAO
	std::vector<VertexBufferBinding> _vertexBuffers;

	// The underlying OpenGL handle that this class is wrapping around
	GLuint _handle;
};
