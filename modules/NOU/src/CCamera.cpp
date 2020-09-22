/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CCamera.cpp
Simple camera component class.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "NOU/CCamera.h"

#include "GLM/gtx/transform.hpp"

namespace nou
{
	Entity* CCamera::current = nullptr;

	CCamera::CCamera(Entity& owner)
	{
		//If there is no camera currently in use (i.e., we are the first one being created)
		//then set the current camera owner to this camera's entity.
		if (current == nullptr)
			current = &owner;

		m_owner = &owner;

		//Initialize our projection and view matrices to identity.
		m_projection = glm::mat4(1.0f);
		m_viewProjection = glm::mat4(1.0f);
	}

	CCamera::~CCamera()
	{
		//If we are being destroyed, then make sure we're not registered as the current camera!
		if (current == m_owner)
			current = nullptr;
	}

	void CCamera::Update()
	{
		m_view = glm::inverse(m_owner->transform.RecomputeGlobal());
		m_viewProjection = m_projection * m_view;
	}

	const glm::mat4& CCamera::GetVP()
	{
		return m_viewProjection;
	}

	const glm::mat4& CCamera::GetView()
	{
		return m_view;
	}

	const glm::mat4& CCamera::GetProj()
	{
		return m_projection;
	}

	void CCamera::Ortho(float left, float right, float bottom, float top, float near, float far)
	{
		m_projection = glm::ortho(left, right, bottom, top, near, far);
		Update();
	}

	void CCamera::Perspective(float fovYDegrees, float aspect, float near, float far)
	{
		m_projection = glm::perspective(glm::radians(fovYDegrees), aspect, near, far);
		Update();
	}
}