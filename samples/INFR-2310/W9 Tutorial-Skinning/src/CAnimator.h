/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CAnimator.h
Simple animator component for demonstrating skeletal animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/
#pragma once

#include "NOU/Entity.h"
#include "Animation.h"

namespace nou
{
	class CAnimator
	{
		public:

		CAnimator(Entity& owner, const SkeletalAnim& anim);
		virtual ~CAnimator() = default;

		CAnimator(CAnimator&&) = default;
		CAnimator& operator=(CAnimator&&) = default;

		SkeletalAnimClip* GetClip();

		void Update(float deltaTime);

		protected:

		Entity* m_owner;
		std::unique_ptr<SkeletalAnimClip> m_clip;
	};
}
