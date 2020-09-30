/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CPathAnimator.h
Simple component for animating an object along a path.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "NOU/Entity.h"
#include "Tools/PathUtility.h"

namespace nou
{
	class CPathAnimator
	{
		public:

		//How long it should take us to travel along each segment.
		float m_segmentTravelTime;

		CPathAnimator(Entity& owner);
		~CPathAnimator() = default;

		void SetMode(PathSampler::PathMode mode);
		void Update(const PathSampler::KeypointSet& keypoints, float deltaTime);

		private:

		Entity* m_owner;
		float m_segmentTimer;
		size_t m_segmentIndex;	
		PathSampler::PathMode m_mode;
	};
}
