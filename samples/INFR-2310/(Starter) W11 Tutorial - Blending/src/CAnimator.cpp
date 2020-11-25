/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CAnimator.cpp
Simple animator component for demonstrating skeletal animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CAnimator.h"
#include "CSkinnedMeshRenderer.h"

namespace nou
{
	CAnimator::CAnimator(Entity& owner)
	{
		m_owner = &owner;

		CSkinnedMeshRenderer& rend = m_owner->Get<CSkinnedMeshRenderer>();
		m_blendTree = std::make_unique<Blendtree>(rend.GetSkeleton());
	}

	Blendtree* CAnimator::GetBlendtree()
	{
		return m_blendTree.get();
	}

	void CAnimator::Update(float deltaTime)
	{
		CSkinnedMeshRenderer& rend = m_owner->Get<CSkinnedMeshRenderer>();
		Skeleton& skeleton = rend.GetSkeleton();

		m_blendTree->Update(deltaTime, skeleton);
		m_blendTree->Apply(skeleton);
		skeleton.DoFK();
		rend.UpdateJointMatrices();
	}
}