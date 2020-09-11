/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Transform.cpp
Simple class for managing object transforms with basic hierarchy support.
*/

#include "NOU/Transform.h"

#include "GLM/gtx/transform.hpp"

namespace nou
{
	Transform::Transform()
	{
		m_parent = nullptr;

		m_pos = glm::vec3(0.0f);
		m_scale = glm::vec3(1.0f);
		m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		m_global = glm::mat4(1.0f);
	}

	Transform::~Transform()
	{
		SetParent(nullptr);
	}

	void Transform::DoFK()
	{
		//First, grab our local transform...
		glm::mat4 local = glm::translate(m_pos) *
						  glm::toMat4(glm::normalize(m_rotation)) *
						  glm::scale(m_scale);

		//If we have a parent, we need to multiply by our parent's
		//global transform.
		if (m_parent != nullptr)
			m_global = m_parent->m_global * local;

		//If we have no parent object, our global transform is our
		//local transform!
		else
			m_global = local;

		//FK is recursive - we now repeat this process on our child nodes.
		//Eventually, we'll be at the bottom of the hierarchy and this will
		//return.
		for (auto* child : m_children)
		{
			child->DoFK();
		}
	}

	const glm::mat4& Transform::RecomputeGlobal()
	{
		//Just as with FK, compute our local, then multiply with
		//our parent's transform if applicable.

		glm::mat4 local = glm::translate(m_pos) *
						  glm::toMat4(m_rotation) *
						  glm::scale(m_scale);

		if (m_parent != nullptr)
			m_global = m_parent->RecomputeGlobal() * local;
		else
			m_global = local;

		return m_global;
	}

	const glm::mat4& Transform::GetGlobal() const
	{
		return m_global;
	}

	glm::mat3 Transform::GetNormal() const
	{
		//The normal matrix is used to transform the normals of our mesh
		//for correct lighting.
		//Basically, we need to orient the normals and undo any non-uniform scaling
		//to prevent strange artifacts (since normals are just directions.)
		//If we're using a uniform scale, then we can just pass the top 3x3 of our
		//transform matrix (the rotation/scale bit) - since we'll re-normalize
		//the normals in our shader anyways.
		if(m_scale.x == m_scale.y && m_scale.x == m_scale.z)
			return glm::mat3(m_global);

		//If we do have a non-uniform scale, then we need to undo that scale, 
		//hence the inverse. However, we want to preserve our rotation.
		//Since the inverse of a rotation matrix IS its transpose, by adding
		//in the transpose we can effectively spit our rotation matrix with
		//the inverse of our scale applied.
		//You could also do some trickery here with the reciprocal of your
		//scale vector and your rotation quaternion, but this is a bit more
		//"bulletproof" and straightforward if you're doing oddball transformations.
		return glm::inverse(glm::transpose(glm::mat3(m_global)));
	}

	void Transform::SetParent(Transform* parent)
	{
		//If we had a parent before, remove this as a child from that object.
		if(m_parent != nullptr)
			m_parent->RemoveChild(this);

		m_parent = parent;

		//If we have a parent now, add this as a child to that object.
		if(m_parent != nullptr)
			m_parent->AddChild(this);			
	}

	void Transform::AddChild(Transform* child)
	{
		m_children.push_back(child);
	}

	void Transform::RemoveChild(Transform* child)
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			if (*it == child)
			{
				m_children.erase(it);
				break;
			}

		}
	}
}