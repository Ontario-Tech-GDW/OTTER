/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Material.cpp
Very basic class for managing the shader program and textures
used by a model.
*/

#include "NOU/Material.h"

namespace nou
{
	Material::Material(const ShaderProgram& program)
	{
		m_program = &program;
		m_curSlot = GL_TEXTURE0;

		//Default to white.
		m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	bool Material::AddTexture(const std::string& name, const Texture2D& tex)
	{
		if(m_curSlot > MAX_SLOT)
			return false;

		GLenum slot = m_curSlot;
		GLint loc = m_program->GetUniformLoc(name);

		m_tex.push_back({ slot, loc, tex.GetID() });

		//Keep track of which GL texture slots we've already used for this material.
		++m_curSlot;

		return true;
	}

	void Material::Use()
	{
		m_program->Bind();

		m_program->SetUniform("matColor", m_color);

		//Bind the textures used by this material.
		for (auto& t : m_tex)
		{
			glUniform1i(t.loc, t.slot);
			glActiveTexture(t.slot - GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, t.id);
		}
	}
}