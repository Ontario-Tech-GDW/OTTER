#include "MeshFactory.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/euler_angles.hpp>
#include <unordered_map>

#include "Logging.h"

#define M_PI 3.14159265359f

typedef VertexPosNormTexCol Vertex;

int AddMiddlePoint(uint32_t offset, glm::vec3 scale, glm::vec3 center, int a, int b, std::vector<Vertex>& vertices, std::unordered_map<uint64_t, uint32_t>& midpointCache)
{
	uint64_t key = 0;
	if (a < b) {
		key = (static_cast<uint64_t>(a) << 32ul) | static_cast<uint64_t>(b);
	}
	else {
		key = (static_cast<uint64_t>(b) << 32ul) | static_cast<uint64_t>(a);
	}
	auto it = midpointCache.find(key);
	if (it != midpointCache.end())
		return it->second;
	else {
		Vertex p1 = vertices[a];
		Vertex p2 = vertices[b];
		Vertex interpolated;
		glm::vec3 pos = glm::normalize((p1.Normal + p2.Normal) / 2.0f);
		float u = atan2f(pos.y, pos.x) / (2.0f * M_PI);
		float v = (asinf(pos.z) / M_PI) + 0.5f;
		interpolated.Position = center + (pos * scale);
		interpolated.Color = (p1.Color + p2.Color) / 2.0f;
		interpolated.Normal = pos;
		interpolated.UV = glm::vec2(u, v);

		int ix = vertices.size();
		midpointCache[key] = ix;
		vertices.push_back(interpolated);
		return ix;
	}
}

void CorrectUVSeams(std::vector<Vertex>& verts, std::vector<uint32_t>& indices, size_t offset) {
	// lambda closure to easily add a vertex with unique texture coordinate to our mesh
	auto addVertex = [&](size_t ix, const glm::vec2& uv) {
		const uint32_t index = indices[ix];
		indices[ix] = (uint32_t)verts.size();
		Vertex vert = verts[index];
		vert.UV = uv;
		verts.push_back(vert);
	};

	// fix texture seams (this is where the magic happens) TODO: fix seams on tops and bottoms
	const size_t current_tris = offset / 3;
	const size_t numTriangles = (indices.size() - offset) / 3;
	// Iterate over all the triangles we added 
	for (size_t i = current_tris; i < current_tris + numTriangles; ++i) {
		// Get the UV coordinates
		const glm::vec2& uv0 = verts[indices[i * 3 + 0]].UV;
		const glm::vec2& uv1 = verts[indices[i * 3 + 1]].UV;
		const glm::vec2& uv2 = verts[indices[i * 3 + 2]].UV;

		const float d1 = uv1.x - uv0.x;
		const float d2 = uv2.x - uv0.x;

		if (abs(d1) > 0.5f && abs(d2) > 0.5f)
			addVertex(i * 3 + 0, uv0 + glm::vec2((d1 > 0.0f) ? 1.0f : -1.0f, 0.0f));
		else if (abs(d1) > 0.5f)
			addVertex(i * 3 + 1, uv1 + glm::vec2((d1 < 0.0f) ? 1.0f : -1.0f, 0.0f));
		else if (abs(d2) > 0.5f)
			addVertex(i * 3 + 2, uv2 + glm::vec2((d2 < 0.0f) ? 1.0f : -1.0f, 0.0f));
	}
}

/// <summary>
/// Calculates the position, normal, and UV coords for a vertex on a sphere (used for iso-sphere generation)
/// </summary>
/// <param name="norm">The normal of the vertex to calculate</param>
/// <param name="scale">The scale of the sphere</param>
/// <param name="center">The center point of the sphere</param>
/// <returns>A vertex with the position, normal, and UV components set</returns>
Vertex CalculateSphereVert(glm::vec3 norm, glm::vec3 scale, glm::vec3 center) {
	Vertex vert;
	glm::vec3 pos = glm::normalize(norm);
	float u = atan2f(pos.y, pos.x) / (2.0f * M_PI);
	float v = (asinf(pos.z) / M_PI) + 0.5f;
	vert.Position = center + (pos * scale);
	vert.Normal = pos;
	vert.UV = glm::vec2(u, v);
	return vert;
}

void MeshFactory::AddIcoSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, float radius, int tessellation, const glm::vec4& col) {
	AddIcoSphere(data, center, glm::vec3(radius), tessellation, col);
}

void MeshFactory::AddIcoSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, const glm::vec3& radii, int tessellation, const glm::vec4& col) {
	LOG_ASSERT(tessellation >= 0, "Tessellation must be greater than zero!");

	uint32_t indexOffset = data.GetVertexCount();
	uint32_t initialIndex = data.GetIndexCount();
	std::vector<glm::ivec3> faces;

	uint32_t estimatedFaces = 12 + pow(tessellation + 1, 3);

	float t = (1.0f + sqrtf(5.0f)) / 2.0f;

	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(-1, t, 0), radii, center));
	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(1, t, 0), radii, center));
	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(-1, -t, 0), radii, center));
	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(1, -t, 0), radii, center));

	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(0, -1, t), radii, center));
	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(0, 1, t), radii, center));
	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(0, -1, -t), radii, center));
	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(0, 1, -t), radii, center));

	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(t, 0, -1), radii, center));
	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(t, 0, 1), radii, center));
	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(-t, 0, -1), radii, center));
	data._vertices.emplace_back(CalculateSphereVert(glm::vec3(-t, 0, 1), radii, center));

	glm::ivec3 iOff = glm::ivec3(indexOffset);

	// 5 faces around point 0
	faces.emplace_back(iOff + glm::ivec3(0, 11, 5));
	faces.emplace_back(iOff + glm::ivec3(0, 5, 1));
	faces.emplace_back(iOff + glm::ivec3(0, 1, 7));
	faces.emplace_back(iOff + glm::ivec3(0, 7, 10));
	faces.emplace_back(iOff + glm::ivec3(0, 10, 11));

	// 5 adjacent faces
	faces.emplace_back(iOff + glm::ivec3(1, 5, 9));
	faces.emplace_back(iOff + glm::ivec3(5, 11, 4));
	faces.emplace_back(iOff + glm::ivec3(11, 10, 2));
	faces.emplace_back(iOff + glm::ivec3(10, 7, 6));
	faces.emplace_back(iOff + glm::ivec3(7, 1, 8));

	// 5 faces around point 3
	faces.emplace_back(iOff + glm::ivec3(3, 9, 4));
	faces.emplace_back(iOff + glm::ivec3(3, 4, 2));
	faces.emplace_back(iOff + glm::ivec3(3, 2, 6));
	faces.emplace_back(iOff + glm::ivec3(3, 6, 8));
	faces.emplace_back(iOff + glm::ivec3(3, 8, 9));

	// 5 adjacent faces
	faces.emplace_back(iOff + glm::ivec3(4, 9, 5));
	faces.emplace_back(iOff + glm::ivec3(2, 4, 11));
	faces.emplace_back(iOff + glm::ivec3(6, 2, 10));
	faces.emplace_back(iOff + glm::ivec3(8, 6, 7));
	faces.emplace_back(iOff + glm::ivec3(9, 8, 1));

	// Cache used to index our midpoints
	std::unordered_map<uint64_t, uint32_t> midPointCache;

	for (int ix = 0; ix < tessellation; ix++)
	{
		std::vector<glm::ivec3> tempFaces;
		for (auto& indices : faces)
		{
			uint32_t a = AddMiddlePoint(indexOffset, radii, center, indices[0], indices[1], data._vertices, midPointCache);
			uint32_t b = AddMiddlePoint(indexOffset, radii, center, indices[1], indices[2], data._vertices, midPointCache);
			uint32_t c = AddMiddlePoint(indexOffset, radii, center, indices[2], indices[0], data._vertices, midPointCache);

			tempFaces.emplace_back(glm::ivec3(indices[0], a, c));
			tempFaces.emplace_back(glm::ivec3(indices[1], b, a));
			tempFaces.emplace_back(glm::ivec3(indices[2], c, b));
			tempFaces.emplace_back(glm::ivec3(a, b, c));
		}
		faces = tempFaces;
	}

	for (auto& face : faces) {
		data._indices.push_back(face[0]);
		data._indices.push_back(face[1]);
		data._indices.push_back(face[2]);
	}

	// Apply color to all verts we added
	for (int ix = indexOffset; ix < data._vertices.size(); ix++) {
		data._vertices[ix].Color = col;
	}

	CorrectUVSeams(data._vertices, data._indices, initialIndex);

}

void MeshFactory::AddUvSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, float radius, int tessellation, const glm::vec4& col) {
	AddUvSphere(data, center, glm::vec3(radius), tessellation, col);
}

void MeshFactory::AddUvSphere(MeshBuilder<VertexPosNormTexCol>& data, const glm::vec3& center, const glm::vec3& radii, int tessellation, const glm::vec4& col) {
	LOG_ASSERT(tessellation >= 0, "Tessellation must be greater than zero!");
	int slices = 1 + pow(2, tessellation + 1);
	int stacks = (slices / 2) + 1;


	int numverts = (slices + 1) * (slices - 1) + 2;
	std::vector<Vertex>& verts = data._vertices;

	uint32_t offset = verts.size();
	uint32_t initialIndex = data._indices.size();
	verts.reserve(verts.size() + numverts);

	float stackAngle, sliceAngle;
	float x, y, z, xy;

	float dLong = (M_PI * 2) / slices;
	float dLat = M_PI / stacks;

	for (int i = 0; i <= stacks; ++i) {
		stackAngle = M_PI / 2.0f - i * dLat;
		xy = cosf(stackAngle);
		z = sinf(stackAngle);

		for (int j = 0; j <= slices; ++j) {
			sliceAngle = j * dLong;

			x = xy * cosf(sliceAngle);
			y = xy * sinf(sliceAngle);

			Vertex vert;
			vert.Normal.x = x;
			vert.Normal.y = y;
			vert.Normal.z = z;
			vert.Position = center + (vert.Normal * radii);
			float u = (float)j / slices;
			float v = 1.0f - (float)i / stacks;
			vert.UV = { u, v };
			verts.push_back(vert);
		}
	}
	verts[offset].UV = { 0.5f, 1.0f };
	verts[verts.size() - 1].UV = { 0.5f, 0.0f };

	// Apply color to all verts we added
	for(int ix = offset; ix < verts.size(); ix++) {
		verts[ix].Color = col;
	}
	
	int numIndices = (slices - 1) * slices * 6;
	data._indices.reserve(data._indices.size() + numIndices);

	// Body loop
	int k1, k2;
	for (int i = 0; i < stacks; ++i)
	{
		k1 = i * (slices + 1);
		k2 = k1 + slices + 1;
		for (int j = 0; j < slices; ++j, ++k1, ++k2)
		{
			// Our top loop
			if (i != 0) {
				data._indices.push_back(offset + k1);
				data._indices.push_back(offset + k2);
				data._indices.push_back(offset + k1 + 1);
			}

			// Everything but our bottom loop
			if (i != (stacks - 1)) {
				data._indices.push_back(offset + k1 + 1);
				data._indices.push_back(offset + k2);
				data._indices.push_back(offset + k2 + 1);
			}
		}
	}
}

void MeshFactory::AddPlane(MeshBuilder<VertexPosNormTexCol>& mesh, const glm::vec3& pos, const glm::vec3& normal,
	const glm::vec3& tangent, const glm::vec2& scale, const glm::vec4& col)
{
	glm::vec3 nNorm = glm::normalize(normal);
	glm::vec3 nTangent = glm::normalize(tangent);
	glm::vec3 binormal = glm::cross(nNorm, nTangent);
	glm::vec2 hScale = scale / 2.0f;
	glm::vec3 positions[4];
	positions[0] = pos - (nTangent * hScale.x) - (binormal * hScale.y);
	positions[1] = pos - (nTangent * hScale.x) + (binormal * hScale.y);
	positions[2] = pos + (nTangent * hScale.x) + (binormal * hScale.y);
	positions[3] = pos + (nTangent * hScale.x) - (binormal * hScale.y);

	glm::vec2 uvs[] = {
		glm::vec2(0.0f, 0.0f), // 0
		glm::vec2(0.0f, 1.0f), // 1
		glm::vec2(1.0f, 1.0f), // 2
		glm::vec2(1.0f, 0.0f), // 3
	};

	const uint32_t p1 = mesh.AddVertex(positions[0], nNorm, uvs[0], col);
	const uint32_t p2 = mesh.AddVertex(positions[1], nNorm, uvs[1], col);
	const uint32_t p3 = mesh.AddVertex(positions[2], nNorm, uvs[2], col);
	const uint32_t p4 = mesh.AddVertex(positions[3], nNorm, uvs[3], col);

	mesh.AddIndexTri(p1, p3, p2);
	mesh.AddIndexTri(p1, p4, p3);
}

void MeshFactory::InvertFaces(MeshBuilder<VertexPosNormTexCol>& mesh)
{
	if (mesh.GetIndexCount() > 0) {
		for (size_t ix = 0; ix < mesh._indices.size(); ix += 3) {
			uint32_t temp = mesh._indices[ix + 2u];
			mesh._indices[ix + 2u] = mesh._indices[ix + 1u];
			mesh._indices[ix + 1u] = temp;
		}
	} else {
		VertexPosNormTexCol temp;
		for (size_t ix = 0; ix < mesh._vertices.size(); ix += 3) {
			temp = mesh._vertices[ix + 2u];
			mesh._vertices[ix + 2u] = mesh._vertices[ix + 1u];
			mesh._vertices[ix + 1u] = temp;
		}
	}
}

void MeshFactory::AddCube(MeshBuilder<VertexPosNormTexCol>& mesh, const glm::vec3& pos, const glm::vec3& scale,
	const glm::vec3& eulerDeg, const glm::vec4& col) {
	const glm::mat4 transform = glm::translate(MAT4_IDENTITY, pos) * glm::mat4(glm::quat(glm::radians(eulerDeg))) * glm::scale(MAT4_IDENTITY, scale);
	AddCube(mesh, transform, col);
}

void MeshFactory::AddCube(MeshBuilder<VertexPosNormTexCol>& mesh, const glm::mat4& transform, const glm::vec4& col) {
	glm::vec3 positions[] = {
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3(0.5f,  0.5f, -0.5f),

		glm::vec3(-0.5f, -0.5f,  0.5f),
		glm::vec3(0.5f, -0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f),
		glm::vec3(0.5f,  0.5f,  0.5f),
	};
	for (auto& position : positions) {
		position = transform * glm::vec4(position, 1.0f);
	}

	glm::vec3 normals[] = {
		glm::vec3(-1.0f,  0.0f,  0.0f), //0
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f), //2
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(0.0f,  0.0f, -1.0f), //4
		glm::vec3(0.0f,  0.0f,  1.0f),
	};
	glm::mat3 rotation = transform; // Truncate the transform to just the rotation component for normals
	for (auto& normal : normals) {
		normal = rotation * normal;
	}

	glm::vec2 uvs[] = {
		glm::vec2(1.0f, 0.0f), // 0
		glm::vec2(1.0f, 1.0f), // 1
		glm::vec2(0.0f, 1.0f), // 2
		glm::vec2(0.0f, 0.0f), // 3
	};

	uint32_t indices[36];

	#pragma region AddVerts

	mesh.ReserveVertexSpace(24);

	// Bottom
	indices[0] = mesh.AddVertex(positions[0], normals[4], uvs[3], col);
	indices[1] = mesh.AddVertex(positions[2], normals[4], uvs[2], col);
	indices[2] = mesh.AddVertex(positions[3], normals[4], uvs[1], col);
	indices[3] = mesh.AddVertex(positions[1], normals[4], uvs[0], col);
	// Top
	indices[4] = mesh.AddVertex(positions[6], normals[5], uvs[0], col);
	indices[5] = mesh.AddVertex(positions[4], normals[5], uvs[1], col);
	indices[6] = mesh.AddVertex(positions[5], normals[5], uvs[2], col);
	indices[7] = mesh.AddVertex(positions[7], normals[5], uvs[3], col);

	// Left
	indices[8]  = mesh.AddVertex(positions[0], normals[0], uvs[0], col);
	indices[9]  = mesh.AddVertex(positions[4], normals[0], uvs[1], col);
	indices[10] = mesh.AddVertex(positions[6], normals[0], uvs[2], col);
	indices[11] = mesh.AddVertex(positions[2], normals[0], uvs[3], col);
	// Right
	indices[12] = mesh.AddVertex(positions[3], normals[1], uvs[0], col);
	indices[13] = mesh.AddVertex(positions[7], normals[1], uvs[1], col);
	indices[14] = mesh.AddVertex(positions[5], normals[1], uvs[2], col);
	indices[15] = mesh.AddVertex(positions[1], normals[1], uvs[3], col);

	// Front
	indices[16] = mesh.AddVertex(positions[2], normals[3], uvs[0], col);
	indices[17] = mesh.AddVertex(positions[6], normals[3], uvs[1], col);
	indices[18] = mesh.AddVertex(positions[7], normals[3], uvs[2], col);
	indices[19] = mesh.AddVertex(positions[3], normals[3], uvs[3], col);
	// Back
	indices[20] = mesh.AddVertex(positions[1], normals[2], uvs[0], col);
	indices[21] = mesh.AddVertex(positions[5], normals[2], uvs[1], col);
	indices[22] = mesh.AddVertex(positions[4], normals[2], uvs[2], col);
	indices[23] = mesh.AddVertex(positions[0], normals[2], uvs[3], col);

	#pragma endregion

	#pragma region Make Triangles

	for (int ix = 0; ix < 6; ix++) {
		size_t o = ix * 4;
		mesh.AddIndexTri(indices[o + 0], indices[o + 1], indices[o + 2]);
		mesh.AddIndexTri(indices[o + 0], indices[o + 2], indices[o + 3]);
	}

	#pragma endregion
}
