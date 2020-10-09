#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "VertexArrayObject.h"
#include "MeshBuilder.h"
#include "VertexTypes.h"

class MeshFactory
{
public:
	/// <summary>
	/// Adds a cube to the given mesh
	/// </summary>
	/// <param name="mesh">The mesh to add the cube to</param>
	/// <param name="pos">The position of the center of the cube in object space</param>
	/// <param name="scale">The scaling to apply to the cube (by default, the cube is 1 unit cubed)</param>
	/// <param name="eulerDeg">The euler angles to rotate the cube by (in degrees)</param>
	/// <param name="col">The color of the cube to add</param>
	static void AddCube(MeshBuilder<VertexPosNormTexCol>& mesh, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& eulerDeg = glm::vec3(0.0f), const glm::vec4& col = glm::vec4(1.0f));
	/// <summary>
	/// Adds a cube to the given mesh using a transformation matrix to orient it in world space
	/// </summary>
	/// <param name="mesh">The mesh to add the cube to</param>
	/// <param name="transform">The matrix to apply to a 1x1x1 cube centered around the origin</param>
	/// <param name="col">The color of the cube to add</param>
	static void AddCube(MeshBuilder<VertexPosNormTexCol>& mesh, const glm::mat4& transform, const glm::vec4& col = glm::vec4(1.0f));

	/// <summary>
	/// Adds an ico-sphere to this mesh, which is tessellated from a set of triangles
	/// </summary>
	/// <param name="data">The mesh to add the cube to</param>
	/// <param name="center">The center of the sphere, in object space</param>
	/// <param name="radius">The radius of the sphere</param>
	/// <param name="tessellation">The number of tessellation steps to apply</param>
	/// <param name="col">The color of the sphere</param>
	static void AddIcoSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, float radius, int tessellation = 0, const glm::vec4& col = glm::vec4(1.0f));
	/// <summary>
	/// Adds an ico-sphere to this mesh, which is tessellated from a set of triangles, scaling it by variable radii
	/// </summary>
	/// <param name="data">The mesh to add the cube to</param>
	/// <param name="center">The center of the sphere, in object space</param>
	/// <param name="radii">The radii of the sphere along each axis</param>
	/// <param name="tessellation">The number of tessellation steps to apply</param>
	/// <param name="col">The color of the sphere</param>
	static void AddIcoSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, const glm::vec3& radii, int tessellation = 0, const glm::vec4& col = glm::vec4(1.0f));
	/// <summary>
	/// Adds a UV sphere to the mesh, where tessellation indicates the number of subdivisions
	/// along the latitude/longitude (starting from 3 subdivisions)
	/// </summary>
	/// <param name="data">The mesh to add the cube to</param>
	/// <param name="center">The center of the sphere, in object space</param>
	/// <param name="radius">The radius of the sphere</param>
	/// <param name="tessellation">The number of tessellation steps to apply</param>
	/// <param name="col">The color of the sphere</param>
	static void AddUvSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, float radius, int tessellation = 0, const glm::vec4& col = glm::vec4(1.0f));
	/// <summary>
	/// Adds a UV sphere to the mesh, where tessellation indicates the number of subdivisions
	/// along the latitude/longitude (starting from 3 subdivisions), scaling it by variable radii
	/// </summary>
	/// <param name="data">The mesh to add the cube to</param>
	/// <param name="center">The center of the sphere, in object space</param>
	/// <param name="radii">The radii of the sphere along each axis</param>
	/// <param name="tessellation">The number of tessellation steps to apply</param>
	/// <param name="col">The color of the sphere</param>
	static void AddUvSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, const glm::vec3& radii, int tessellation = 0, const glm::vec4& col = glm::vec4(1.0f));

	/// <summary>
	/// Adds a plane to the mesh, centered at a position and facing towards a normal and along a tangent
	/// </summary>
	/// <param name="mesh">The mesh to add the cube to</param>
	/// <param name="pos">The center of the mesh in object space</param>
	/// <param name="normal">The direction that the plane is facing</param>
	/// <param name="tangent">The vector along which the local x-axis of the plane is aligned</param>
	/// <param name="scale">The size of the plane, with x being along tangent, and y being along normal X tangent</param>
	/// <param name="col">The color of the plane</param>
	static void AddPlane(MeshBuilder<VertexPosNormTexCol>& mesh, const glm::vec3& pos, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec2& scale, const glm::vec4& col = glm::vec4(1.0f));

	
protected:	
	MeshFactory() = default;
	~MeshFactory() = default;

	inline static const glm::mat4 MAT4_IDENTITY = glm::mat4(1.0f);
};