/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMorphMeshRenderer.cpp
Simple renderer component for demonstrating morph target animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CMorphMeshRenderer.h"
#include "NOU/CCamera.h"

namespace nou
{
	CMorphMeshRenderer::CMorphMeshRenderer(Entity& owner, const Mesh& baseMesh, Material& mat)
	{
		m_owner = &owner;
		m_mat = &mat;
		m_vao = std::make_unique<VertexArray>();

		const VertexBuffer* vboUV;

		//UVs won't change with morph target animation.
		//If our base mesh has them, we'll just associate those now.
		if((vboUV = baseMesh.GetVBO(Mesh::Attrib::UV)))
			m_vao->BindAttrib(*vboUV, static_cast<GLint>(Attrib::UV));

		UpdateData(baseMesh, baseMesh, 0.0f);
	}

	void CMorphMeshRenderer::UpdateData(const Mesh& frame0, const Mesh& frame1, float t)
	{
		const VertexBuffer* vbo;

		//TODO: What needs to happen here?

		m_t = t;
	}

	void CMorphMeshRenderer::Draw()
	{
		m_mat->Use();

		auto& transform = m_owner->transform;

		//We are assuming the names used by uniform shader variables as a convention here.
		//In a larger project, we would have a more elegant system for registering
		//or even automatically detecting uniform names.

		//TODO: What do we need to send to our shader program?

		m_vao->Draw();
	}
}