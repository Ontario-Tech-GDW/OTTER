/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CSkinnedMeshRenderer.h
Simple skinned mesh renderer component.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "NOU/CMeshRenderer.h"
#include "SkinnedMesh.h"

#include <memory>

namespace nou
{
	class CSkinnedMeshRenderer : CMeshRenderer
	{
		public:

		CSkinnedMeshRenderer(Entity& owner,
						     const SkinnedMesh& mesh,
							 Material& mat);
		virtual ~CSkinnedMeshRenderer() = default;

		CSkinnedMeshRenderer(CSkinnedMeshRenderer&&) = default;
		CSkinnedMeshRenderer& operator=(CSkinnedMeshRenderer&&) = default;

		Skeleton& GetSkeleton();

		void SetMesh(const SkinnedMesh& mesh);
		virtual void Draw();

		protected:

		std::unique_ptr<Skeleton> m_skeleton;
	};	
}