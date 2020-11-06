#pragma once

#include <GLM/glm.hpp>
#include "Graphics/VertexArrayObject.h"

struct VertexPosCol {
	glm::vec3 Position;
	glm::vec4 Color;

	VertexPosCol() : Position(glm::vec3(0.0f)), Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}
	VertexPosCol(const glm::vec3& pos, const glm::vec4& col) :
		Position(pos), Color(col) {}
	VertexPosCol(float x, float y, float z, float r, float g, float b, float a = 1.0f) :
		Position({x, y, z}), Color({r, g, b, a}) {}

	static const std::vector<BufferAttribute> V_DECL;
};

struct VertexPosNormCol {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec4 Color;

	VertexPosNormCol() : Position(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}
	VertexPosNormCol(const glm::vec3& pos, const glm::vec3& norm, const glm::vec4& col) :
		Position(pos), Normal(norm), Color(col) {}
	VertexPosNormCol(float x, float y, float z, float nX, float nY, float nZ, float r, float g, float b, float a = 1.0f) :
		Position({ x, y, z }), Normal({nX, nY, nZ}), Color({ r, g, b, a }) {}
	
	static const std::vector<BufferAttribute> V_DECL;
};

struct VertexPosNormTex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;

	VertexPosNormTex() : Position(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), UV(glm::vec2(0.0f, 0.0f)) {}
	VertexPosNormTex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv) :
		Position(pos), Normal(norm), UV(uv) {}
	VertexPosNormTex(float x, float y, float z, float nX, float nY, float nZ, float u, float v) :
		Position({ x, y, z }), Normal({ nX, nY, nZ }), UV({ u, v }) {}

	static const std::vector<BufferAttribute> V_DECL;
};

struct VertexPosNormTexCol {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::vec4 Color;

	VertexPosNormTexCol() : Position(glm::vec3(0.0f)), Normal(glm::vec3(0.0f)), UV(glm::vec2(0.0f, 0.0f)), Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {}
	VertexPosNormTexCol(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv, const glm::vec4& col) :
		Position(pos), Normal(norm), UV(uv), Color(col) {}
	VertexPosNormTexCol(float x, float y, float z, float nX, float nY, float nZ, float u, float v, float r, float g, float b, float a = 1.0f) :
		Position({ x, y, z }), Normal({ nX, nY, nZ }), UV({ u, v }), Color({r, g, b, a}) {}

	static const std::vector<BufferAttribute> V_DECL;
};