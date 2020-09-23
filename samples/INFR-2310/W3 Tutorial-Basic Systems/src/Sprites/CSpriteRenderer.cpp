/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CSpriteRenderer.cpp
Simple sprite renderer component.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CSpriteRenderer.h"
#include "NOU/CCamera.h"
#include "NOU/Mesh.h"

namespace nou
{
	CSpriteRenderer::CSpriteRenderer(Entity& owner, Spritesheet& sheet, Material& mat)
	{
		m_owner = &owner;
		m_sheet = &sheet;
		m_mat = &mat;

		m_vao = std::make_unique<VertexArray>();

		SetSize(m_sheet->GetFrameSize());
		SetFrame(m_sheet->GetDefaultFrame());

		m_vao->BindAttrib(*m_vboVert, (GLint)Mesh::Attrib::POSITION);
		m_vao->BindAttrib(*m_vboUV, (GLint)Mesh::Attrib::UV);
	}

	void CSpriteRenderer::Draw()
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

	void CSpriteRenderer::SetSize(const glm::vec2& size)
	{
		std::vector<glm::vec3> verts;
		verts.resize(6);

		//Bottom left, bottom right, top right.
		verts[0] = glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f);
		verts[1] = glm::vec3( 0.5f * size.x, -0.5f * size.y, 0.0f);
		verts[2] = glm::vec3( 0.5f * size.x,  0.5f * size.y, 0.0f);

		//Bottom left, top right, top left.
		verts[3] = glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f);
		verts[4] = glm::vec3( 0.5f * size.x,  0.5f * size.y, 0.0f);
		verts[5] = glm::vec3(-0.5f * size.x,  0.5f * size.y, 0.0f);

		if(m_vboVert)
			m_vboVert->UpdateData(verts);
		else
			m_vboVert = std::make_unique<VertexBuffer>(3, verts);
	}

	void CSpriteRenderer::SetFrame(const Spritesheet::Frame& frame)
	{
		std::vector<glm::vec2> uvs;
		uvs.resize(6);

		//Bottom left, bottom right, top right.
		uvs[0] = frame.uv[Spritesheet::VertIndex::BOTTOM_LEFT];
		uvs[1] = frame.uv[Spritesheet::VertIndex::BOTTOM_RIGHT];
		uvs[2] = frame.uv[Spritesheet::VertIndex::TOP_RIGHT];

		//Bottom left, top right, top left.
		uvs[3] = frame.uv[Spritesheet::VertIndex::BOTTOM_LEFT];
		uvs[4] = frame.uv[Spritesheet::VertIndex::TOP_RIGHT];
		uvs[5] = frame.uv[Spritesheet::VertIndex::TOP_LEFT];

		if (m_vboUV)
			m_vboUV->UpdateData(uvs);
		else
			m_vboUV = std::make_unique<VertexBuffer>(2, uvs, true);
	}
}