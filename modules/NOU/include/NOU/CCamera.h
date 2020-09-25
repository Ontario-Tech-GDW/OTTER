/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CCamera.h
Simple camera component class.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "Entity.h"

namespace nou
{
	class CCamera
	{
		public:

		//The entity that owns the camera currently being used.
		//Especially useful if you plan to have more than one camera.
		//(e.g., for UI, portals, security cameras, etc.)
		static Entity* current;

		CCamera(Entity& owner);
		virtual ~CCamera();

		void Update();

		//Returns the viewprojection matrix for rendering.
		//VP = projection * view.
		const glm::mat4& GetVP();

		const glm::mat4& GetView();
		const glm::mat4& GetProj();

		//Sets up an orthographic projection using GLM.
		void Ortho(float left, float right, float bottom, float top, float near, float far);

		//Sets up a 3D perspective projection using GLM.
		void Perspective(float fovYDegrees, float aspect, float near, float far);

		protected:

		Entity* m_owner;
		glm::mat4 m_view;
		glm::mat4 m_projection;
		glm::mat4 m_viewProjection;
	};
}