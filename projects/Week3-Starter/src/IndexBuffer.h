#pragma once
#include "IBuffer.h"
#include <cstdint>
#include <stdexcept>

/// <summary>
/// Represents the element type of an Index Buffer
/// </summary>
enum class IndexType {
	UByte = GL_UNSIGNED_BYTE,
	UShort = GL_UNSIGNED_SHORT,
	UInt = GL_UNSIGNED_INT,
	Unknown = GL_NONE
};

/// <summary>
/// The index buffer will store indices for rendering (uint8_t, uint16_t and uint32_t)
/// </summary>
class IndexBuffer : public IBuffer
{
public:
	/// <summary>
	/// Creates a new index buffer, with the given usage. Data will still need to be uploaded before it can be used
	/// </summary>
	/// <param name="usage">The usage hint for the buffer, default is GL_STATIC_DRAW</param>
	IndexBuffer(BufferUsage usage = BufferUsage::StaticDraw) :
		IBuffer(BufferType::Index, usage), _elementType(IndexType::Unknown) { }

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
	inline void LoadData(const void* data, size_t elementSize, size_t elementCount, IndexType elementType) {
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
	IndexType GetElementType() const { return _elementType; }

	/// <summary>
	/// Unbinds the currently bound index buffer
	/// </summary>
	static void Unbind() { IBuffer::UnBind(BufferType::Index); }

protected:
	IndexType _elementType;
};

// These are all template specializations for LoadData, they are in the .h file cause templates are weird
// Basically, by leaving the template arguments empty and providing explicit types to our parameters, we
// can have specialized behaviour for each of the types, neat!

template<>
inline void IndexBuffer::LoadData<uint8_t>(const uint8_t* data, size_t count) {
	IBuffer::LoadData<uint8_t>(data, count);
	_elementType = IndexType::UByte;
}
template<>
inline void IndexBuffer::LoadData<uint16_t>(const uint16_t* data, size_t count) {
	IBuffer::LoadData<uint16_t>(data, count);
	_elementType = IndexType::UShort;
}
template<>
inline void IndexBuffer::LoadData<uint32_t>(const uint32_t* data, size_t count) {
	IBuffer::LoadData<uint32_t>(data, count);
	_elementType = IndexType::UInt;
}
