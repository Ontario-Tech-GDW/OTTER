#pragma once
#include <vector>
#include <VertexArrayObject.h>

template <typename VertType>
class MeshBuilder
{
public:
	MeshBuilder() :
		_vertices(std::vector<VertType>()),
		_indices(std::vector<uint32_t>()) {}
	~MeshBuilder() = default;

	/// <summary>
	/// Adds a new vertex to the mesh, returning it's index within the vertex buffer
	/// </summary>
	/// <param name="vertex">The vertex to add to the buffer</param>
	/// <returns>The index of the vertex, as can be added to an index buffer</returns>
	uint32_t AddVertex(const VertType& vertex) {
		_vertices.push_back(vertex);
		return static_cast<uint32_t>(_vertices.size() - 1u);
	}

	template<class...Args>
	uint32_t AddVertex(Args&&... args) {
		_vertices.emplace_back(std::forward<Args>(args)...);
		return static_cast<uint32_t>(_vertices.size() - 1u);
	}
	
	/// <summary>
	/// Adds an index to the index buffer
	/// </summary>
	/// <param name="index">The index to append to the buffer</param>
	void AddIndex(uint32_t index) {
		_indices.push_back(index);
	}

	/// <summary>
	/// Adds a triangle between the three indices
	/// </summary>
	/// <param name="a">The index of the first vertex</param>
	/// <param name="b">The index of the second vertex</param>
	/// <param name="c">The index of the third vertex</param>
	void AddIndexTri(uint32_t a, uint32_t b, uint32_t c)
	{
		ReserveIndexSpace(3);
		_indices.push_back(a);
		_indices.push_back(b);
		_indices.push_back(c);
	}
	
	/// <summary>
	/// Resizes the internal vector to allocate space for new vertices, can improve
	/// performance when appending large meshes of a known size
	/// </summary>
	/// <param name="extendAmount">The number of vertices to reserve space for</param>
	void ReserveVertexSpace(size_t extendAmount) {
		_vertices.reserve(_vertices.size() + extendAmount);
	}
	/// <summary>
	/// Resizes the internal vector to allocate space for new indices, can improve
	/// performance when appending large meshes of a known size
	/// </summary>
	/// <param name="extendAmount">The number of indices to reserve space for</param>
	void ReserveIndexSpace(size_t extendAmount) {
		_indices.reserve(_indices.size() + extendAmount);
	}

	/// <summary>
	/// Returns the number of vertices in this mesh
	/// </summary>
	size_t GetVertexCount() const { return _vertices.size(); }
	/// <summary>
	/// Returns the number of indices in this mesh
	/// </summary>
	size_t GetIndexCount() const { return _indices.size(); }
	/// <summary>
	/// Returns the number of triangles in this mesh. If the index vector contains data,
	/// it will calculate the triangle count using that, otherwise it will use the number
	/// of vertices
	/// </summary>
	size_t GetTriangleCount() const { return _indices.size() > 0 ? _indices.size() / 3 : _vertices.size() / 3; }

	VertexArrayObject::sptr Bake() {
		VertexBuffer::sptr vbo = VertexBuffer::Create();
		vbo->LoadData(GetVertexDataPtr(), _vertices.size());

		IndexBuffer::sptr ebo = IndexBuffer::Create();
		ebo->LoadData(GetIndexDataPtr(), _indices.size());

		VertexArrayObject::sptr result = VertexArrayObject::Create();
		result->AddVertexBuffer(vbo, VertType::V_DECL);
		result->SetIndexBuffer(ebo);

		return result;
	}
	
	/// <summary>
	/// Gets a pointer to the underlying vertex data in the mesh, valid only
	/// until another call to AddVertex
	/// </summary>
	const VertType* GetVertexDataPtr() const {
		return _vertices.data();
	}
	/// <summary>
	/// Gets a pointer to the underlying index data in the mesh, valid only
	/// until another call to AddIndex or AddIndexTri
	/// </summary>
	const uint32_t* GetIndexDataPtr() const {
		return _indices.data();
	}
	
protected:
	friend class MeshFactory;
	
	std::vector<VertType> _vertices;
	std::vector<uint32_t> _indices;
};
