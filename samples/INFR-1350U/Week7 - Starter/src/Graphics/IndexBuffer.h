#pragma once
#include "IBuffer.h"
#include <cstdint>
#include <stdexcept>
#include <memory>

/// <summary>
/// The index buffer will store indices for rendering (uint8_t, uint16_t and uint32_t)
/// </summary>
class IndexBuffer : public IBuffer
{
public:
	typedef std::shared_ptr<IndexBuffer> sptr;
	static inline sptr Create(GLenum usage = GL_STATIC_DRAW) {
		return std::make_shared<IndexBuffer>(usage);
	}
	
public:
	/// <summary>
	/// Creates a new index buffer, with the given usage. Data will still need to be uploaded before it can be used
	/// </summary>
	/// <param name="usage">The usage hint for the buffer, default is GL_STATIC_DRAW</param>
	IndexBuffer(GLenum usage = GL_STATIC_DRAW) : 
		IBuffer(GL_ELEMENT_ARRAY_BUFFER, usage), _elementType(GL_NONE) { }

	// We'll override the LoadData to force users to use our overload that takes in the element type as well
	inline void LoadData(const void* data, size_t elementSize, size_t elementCount) override {
		throw std::runtime_error("Must use the templated overload, or the LoadData that specifies the element type");
	}
	/// <summary>
	/// Loads some data into our index buffer, specifying the type of indices we are using via the elementType parameter
	/// </summary>
	/// <param name="data">The pointer to the data to load in</param>
	/// <param name="elementSize">The size of a single element, in bytes</param>
	/// <param name="elementCount">The number of elements to upload</param>
	/// <param name="elementType">The type of elements you are storing (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT)</param>
	inline void LoadData(const void* data, size_t elementSize, size_t elementCount, GLenum elementType) {
		IBuffer::LoadData(data, elementSize, elementCount);
		_elementType = elementType;
	}
	/// <summary>
	/// Loads data of a known type into this index buffer
	/// </summary>
	/// <typeparam name="T">The type of data to load, must be uint8_t, uint16_t or uint32_t</typeparam>
	/// <param name="data">A pointer to the start of the array</param>
	/// <param name="count">The number of elements in the array to upload</param>
	template <typename T>
	void LoadData(const T* data, size_t count) { throw std::runtime_error("Must be one of uint8_t, uint16_t or uint32_t"); } // Note, see template specializations below

	/// <summary>
	/// Gets the underlying index type for this buffer (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT)
	/// </summary>
	GLenum GetElementType() const { return _elementType; }

	/// <summary>
	/// Unbinds the currently bound index buffer
	/// </summary>
	static void UnBind() { IBuffer::UnBind(GL_ELEMENT_ARRAY_BUFFER); }

protected:
	GLenum _elementType;
};

// These are all template specializations for LoadData, they are in the .h file cause templates are weird
// Basically, by leaving the template arguments empty and providing explicit types to our parameters, we
// can have specialized behaviour for each of the types, neat!

template<>
inline void IndexBuffer::LoadData<uint8_t>(const uint8_t* data, size_t count) {
	IBuffer::LoadData<uint8_t>(data, count);
	_elementType = GL_UNSIGNED_BYTE;
}
template<>
inline void IndexBuffer::LoadData<uint16_t>(const uint16_t* data, size_t count) {
	IBuffer::LoadData<uint16_t>(data, count);
	_elementType = GL_UNSIGNED_SHORT;
}
template<>
inline void IndexBuffer::LoadData<uint32_t>(const uint32_t* data, size_t count) {
	IBuffer::LoadData<uint32_t>(data, count);
	_elementType = GL_UNSIGNED_INT;
}
