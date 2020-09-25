/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMeshRenderer.cpp
Simple mesh renderer component - place this on an entity if you
want to draw a 3D mesh for that entity.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "NOU/CMeshRenderer.h"
#include "NOU/CCamera.h"

namespace nou
{
	CMeshRenderer::CMeshRenderer()
	{
		m_owner = nullptr;
		m_mat = nullptr;
		m_vao = nullptr;
	}

	CMeshRenderer::CMeshRenderer(Entity& owner, 
						         const Mesh& mesh,
								 Material& mat)
	{
		m_owner = &owner;
		m_mat = &mat;
		m_vao = std::make_unique<VertexArray>();
		SetMesh(mesh);	
	}

	//This will fetch and bind all of our data (vertices, normals, UVs)
	//to the VAO used for this renderer.
	//Basically, this makes sure that OpenGL will be able to find all of
	//the data needed to draw our 3D model.
	void CMeshRenderer::SetMesh(const Mesh& mesh)
	{
		const VertexBuffer* vbo;

		if ((vbo = mesh.GetVBO(Mesh::Attrib::POSITION)) != nullptr)
			m_vao->BindAttrib(*vbo, (GLint)Mesh::Attrib::POSITION);

		if ((vbo = mesh.GetVBO(Mesh::Attrib::NORMAL)) != nullptr)
			m_vao->BindAttrib(*vbo, (GLint)Mesh::Attrib::NORMAL);

		if ((vbo = mesh.GetVBO(Mesh::Attrib::UV)) != nullptr)
			m_vao->BindAttrib(*vbo, (GLint)Mesh::Attrib::UV);
	}

	void CMeshRenderer::SetMaterial(Material& mat)
	{
		m_mat = &mat;
	}

	void CMeshRenderer::Draw()
	{
		m_mat->Use();

		auto& transform = m_owner->transform;

		//We are assuming the names used by uniform shader variables as a convention here.
		//In a larger project, we would have a more elegant system for registering
		//or even automatically detecting uniform names.
		ShaderProgram::Current()->SetUniform("viewproj", CCamera::current->Get<CCamera>().GetVP());
		ShaderProgram::Current()->SetUniform("model", transform.GetGlobal());
		ShaderProgram::Current()->SetUniform("normal", transform.GetNormal());
		
		m_vao->Draw();
	}
}