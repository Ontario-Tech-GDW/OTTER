/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMorphMeshRenderer.h
Simple renderer component for demonstrating morph target animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "NOU/CMeshRenderer.h"

#include <memory>

namespace nou
{
	class CMorphMeshRenderer : CMeshRenderer
	{
		public:

		enum class Attrib
		{
			POSITION_0 = 0,
			POSITION_1 = 1,
			NORMAL_0 = 2,
			NORMAL_1 = 3,
			UV = 4
		};

		CMorphMeshRenderer(Entity& owner,
			const Mesh& baseMesh,
			Material& mat);
		virtual ~CMorphMeshRenderer() = default;

		CMorphMeshRenderer(CMorphMeshRenderer&&) = default;
		CMorphMeshRenderer& operator=(CMorphMeshRenderer&&) = default;

		void UpdateData(const Mesh& frame0, const Mesh& frame1, float t);
		virtual void Draw();

		protected:

		//Pointers to the frames we're currently in between.
		Mesh* m_frame0;
		Mesh* m_frame1;
		//The t-value for interpolating between our frames.
		float m_t;
	};
}