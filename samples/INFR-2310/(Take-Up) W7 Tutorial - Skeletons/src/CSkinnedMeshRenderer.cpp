/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CSkinnedMeshRenderer.cpp
Simple skinned mesh renderer component.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CSkinnedMeshRenderer.h"
#include "NOU/CCamera.h"

namespace nou
{

	CSkinnedMeshRenderer::CSkinnedMeshRenderer(Entity& owner,
		const SkinnedMesh& mesh,
		Material& mat)
	{
		m_owner = &owner;
		m_mat = &mat;
		m_vao = std::make_unique<VertexArray>();
		m_skeleton = std::make_unique<Skeleton>();

		SetMesh(mesh);
	}

	Skeleton& CSkinnedMeshRenderer::GetSkeleton()
	{
		return *m_skeleton;
	}

	void CSkinnedMeshRenderer::SetMesh(const SkinnedMesh& mesh)
	{
		const VertexBuffer* vbo;

		if ((vbo = mesh.GetVBO(Mesh::Attrib::POSITION)) != nullptr)
			m_vao->BindAttrib(*vbo, (GLint)Mesh::Attrib::POSITION);

		if ((vbo = mesh.GetVBO(Mesh::Attrib::NORMAL)) != nullptr)
			m_vao->BindAttrib(*vbo, (GLint)Mesh::Attrib::NORMAL);

		if ((vbo = mesh.GetVBO(Mesh::Attrib::UV)) != nullptr)
			m_vao->BindAttrib(*vbo, (GLint)Mesh::Attrib::UV);

		//This will make a copy of the skeleton data from our base mesh.
		*m_skeleton = mesh.m_skeleton;
	}

	void CSkinnedMeshRenderer::Draw()
	{
		m_mat->Use();

		auto& transform = m_owner->transform;

		//In addition to what we usually send (model, viewprojection, and normal matrix),
		//we'll eventually be sending our joint matrices as well.
		ShaderProgram::Current()->SetUniform("viewproj", CCamera::current->Get<CCamera>().GetVP());
		ShaderProgram::Current()->SetUniform("model", transform.GetGlobal());
		ShaderProgram::Current()->SetUniform("normal", transform.GetNormal());

		m_vao->Draw();
	}
}