/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CSpriteRenderer.h
Simple sprite renderer component.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "NOU/GLObjects.h"
#include "NOU/Material.h"
#include "NOU/Entity.h"
#include "Spritesheet.h"
#include "GLM/glm.hpp"

#include <memory>

namespace nou
{
	class CSpriteRenderer
	{
		public:

		CSpriteRenderer(Entity& owner, Spritesheet& sheet, Material& mat);
		virtual ~CSpriteRenderer() = default;

		//"Move" constructor and assignment operators required for 
		//using this class as a component (need to specify these manually
		//since we're working with smart pointers as class members).
		CSpriteRenderer(CSpriteRenderer&&) = default;
		CSpriteRenderer& operator=(CSpriteRenderer&&) = default;

		void Draw();

		void SetSize(const glm::vec2& size);
		void SetFrame(const Spritesheet::Frame& frame);

		protected:

		Entity* m_owner;
		Material* m_mat;
		Spritesheet* m_sheet;

		std::unique_ptr<VertexBuffer> m_vboVert;
		std::unique_ptr<VertexBuffer> m_vboUV;

		std::unique_ptr<VertexArray> m_vao;
	};
}
