#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "VertexArrayObject.h"
#include "MeshBuilder.h"
#include "VertexTypes.h"

class MeshFactory
{
public:
	
	static void AddCube(MeshBuilder<VertexPosNormTexCol>& mesh, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& eulerDeg = glm::vec3(0.0f), const glm::vec4& col = glm::vec4(1.0f));
	static void AddCube(MeshBuilder<VertexPosNormTexCol>& mesh, const glm::mat4& transform, const glm::vec4& col = glm::vec4(1.0f));

	static void AddIcoSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, float radius, int tessellation = 0, const glm::vec4& col = glm::vec4(1.0f));
	static void AddIcoSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, const glm::vec3& radii, int tessellation = 0, const glm::vec4& col = glm::vec4(1.0f));
	static void AddUvSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, float radius, int tessellation = 0, const glm::vec4& col = glm::vec4(1.0f));
	static void AddUvSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, const glm::vec3& radii, int tessellation = 0, const glm::vec4& col = glm::vec4(1.0f));

	static void AddPlane(MeshBuilder<VertexPosNormTexCol>& mesh, const glm::vec3& pos, const glm::vec3& normal, const glm::vec3& tangent, const glm::vec2& scale, const glm::vec4& col = glm::vec4(1.0f));

	static void InvertFaces(MeshBuilder<VertexPosNormTexCol>& mesh);
	
protected:	
	MeshFactory() = default;
	~MeshFactory() = default;

	inline static const glm::mat4 MAT4_IDENTITY = glm::mat4(1.0f);
};