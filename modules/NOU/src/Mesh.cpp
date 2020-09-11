/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Mesh.cpp
Very basic class for managing the data associated with a 3D model.
*/

#include "NOU/Mesh.h"

namespace nou
{
	void Mesh::SetVerts(const std::vector<glm::vec3>& verts)
	{
		m_verts = verts;
		SetVBO(Attrib::POSITION, 3, m_verts);
	}

	void Mesh::SetNormals(const std::vector<glm::vec3>& normals)
	{
		m_normals = normals;
		SetVBO(Attrib::NORMAL, 3, m_normals);			
	}

	void Mesh::SetUVs(const std::vector<glm::vec2>& uvs)
	{
		m_uvs = uvs;
		SetVBO(Attrib::UV, 2, m_uvs);
	}

	const VertexBuffer* Mesh::GetVBO(Mesh::Attrib attrib) const
	{
		auto it = m_vbo.find(attrib);

		if (it == m_vbo.end())
			return nullptr;

		return it->second.get();
	}
}