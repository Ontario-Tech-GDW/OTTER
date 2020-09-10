/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Material.h
Very basic class for managing the shader program and textures
used by a model.
*/

#pragma once

#include "Texture.h"
#include "Shader.h"

#include "GLM/glm.hpp"

#include <vector>

namespace nou
{
	class Material
	{
		public:

		glm::vec3 m_color;

		Material(const ShaderProgram& program);
		~Material() = default;

		//Returns true if the texture was added successfully.
		//This will fail if you try to use more than the maximum number of textures.
		//(Which we have set at 16 via our specification of MAX_SLOT).
		bool AddTexture(const std::string& name, const Texture2D& tex);

		//Should be called by the material's user before drawing the object (i.e., mesh).
		void Use();

		protected:

		//Small utility struct for managing how and where OpenGL will deal with our texture(s).
		struct TexUniform
		{
			GLenum slot;
			GLint loc;
			GLuint id;
		};

		GLenum m_curSlot;
		static const GLenum MAX_SLOT = GL_TEXTURE15;

		std::vector<TexUniform> m_tex;
		const ShaderProgram* m_program;
	};
}