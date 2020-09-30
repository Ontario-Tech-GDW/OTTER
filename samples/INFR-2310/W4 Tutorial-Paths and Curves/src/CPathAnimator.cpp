/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CPathAnimator.cpp
Simple component for animating an object along a path.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CPathAnimator.h"


namespace nou
{
	CPathAnimator::CPathAnimator(Entity& owner)
	{
		m_owner = &owner;

		m_segmentIndex = 0;
		m_segmentTimer = 0.0f;

		m_segmentTravelTime = 1.0f;
		m_mode = PathSampler::PathMode::CATMULL;
	}

	void CPathAnimator::SetMode(PathSampler::PathMode mode)
	{
		m_mode = mode;
		m_segmentIndex = 0;
	}

	void CPathAnimator::Update(const PathSampler::KeypointSet& keypoints, float deltaTime)
	{
		if (keypoints.size() == 0 || m_segmentTravelTime == 0)
			return;

		m_segmentTimer += deltaTime;

		//Ensure we are not "over time" and move to the next segment
		//if necessary.
		while (m_segmentTimer > m_segmentTravelTime)
		{
			m_segmentTimer -= m_segmentTravelTime;

			m_segmentIndex += 1;

			if (m_segmentIndex >= keypoints.size())
				m_segmentIndex = 0;
		}

		float t = m_segmentTimer / m_segmentTravelTime;

		//Neither Catmull nor Bezier make sense with less than 4 points.
		if (keypoints.size() < 4)
		{
			m_owner->transform.m_pos = keypoints[0]->transform.m_pos;
			return;
		}

		size_t p0_ind, p1_ind, p2_ind, p3_ind;
		glm::vec3 p0, p1, p2, p3;

		//For Catmull - path segment is between p1 and p2;
		//so our "segment" index is p1.
		p1_ind = m_segmentIndex;
		p0_ind = (p1_ind == 0) ? keypoints.size() - 1 : p1_ind - 1;
		p2_ind = (p1_ind + 1) % keypoints.size();
		p3_ind = (p2_ind + 1) % keypoints.size();

		p0 = keypoints[p0_ind]->transform.m_pos;
		p1 = keypoints[p1_ind]->transform.m_pos;
		p2 = keypoints[p2_ind]->transform.m_pos;
		p3 = keypoints[p3_ind]->transform.m_pos;

		m_owner->transform.m_pos = PathSampler::Catmull(p0, p1, p2, p3, t);

	}
}