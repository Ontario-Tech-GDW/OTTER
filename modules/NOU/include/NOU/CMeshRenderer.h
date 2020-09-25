/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMeshRenderer.h
Simple mesh renderer component - place this on an entity if you 
want to draw a 3D mesh for that entity.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "GLObjects.h"
#include "Mesh.h"
#include "Material.h"
#include "Entity.h"

#include <memory>

namespace nou
{
	class CMeshRenderer
	{
		public:

		CMeshRenderer(Entity& owner, const Mesh& mesh, Material& mat);
		virtual ~CMeshRenderer() = default;

		//This is called a move constructor. Among other things, move constructors
		//are important when an object needs to be part of a container.
		//ENTT uses a container system for components.
		//Normally, this would be generated implicitly.
		//Since we have a std::unique_ptr as a member, we need to tell the compiler
		//to generate one for us.
		//Depending on the class, you may need to write one of these yourself -
		//See GLObjects.h for an example with the VAO class.
		CMeshRenderer(CMeshRenderer&&) = default;

		//This is called a move assignment operator.
		//Same as above, ENTT requires this, and we tell the compiler to generate
		//one for us.
		CMeshRenderer& operator=(CMeshRenderer&&) = default;

		void SetMesh(const Mesh& mesh);
		void SetMaterial(Material& mat);
		virtual void Draw();

		protected:

		Entity* m_owner;
		Material* m_mat;
		std::unique_ptr<VertexArray> m_vao;

		//Having a default constructor makes it easier for us to inherit from
		//this class later on (e.g., for a mesh renderer with skeletal animation).
		//However, it does not make sense to instantiate this class on its own
		//without a mesh, material, etc. - So we make this protected.
		CMeshRenderer();
	};
}