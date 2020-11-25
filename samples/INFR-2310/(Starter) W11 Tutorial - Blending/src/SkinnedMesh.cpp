/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

SkinnedMesh.cpp
Basic classes for managing the data associated with a skinned 3D model.
*/

#include "SkinnedMesh.h"

#include "GLM/gtx/transform.hpp"

namespace nou
{
	Joint::Joint(Skeleton* owner)
	{
		m_name = "NULL";
		m_owner = owner;

		m_global = glm::mat4(1.0f);
		m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
		m_basePos = m_pos;
		m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		m_baseRotation = m_baseRotation;
		m_invBind = glm::mat4(1.0f);

		m_parent = false;
		m_parentInd = 0;
	}

	//Copy a joint's data to a new skeleton.
	Joint::Joint(Skeleton* owner, const Joint& other)
	{
		m_name = other.m_name;
		m_owner = owner;

		m_global = other.m_global;
		m_pos = other.m_pos;
		m_basePos = other.m_basePos;
		m_rotation = other.m_rotation;
		m_baseRotation = other.m_baseRotation;
		m_invBind = other.m_invBind;

		m_parent = other.m_parent;
		m_parentInd = other.m_parentInd;

		m_childrenInd = other.m_childrenInd;
	}

	//Forward kinematics.
	void Joint::DoFK()
	{
		//If we don't have a skeleton, something is wrong.
		if(m_owner == nullptr)
			return;

		//Local transform...
		glm::mat4 local = glm::translate(m_pos) *
						  glm::toMat4(glm::normalize(m_rotation));

		//If we have a parent, get its global transform
		//and concatenate with our local transform to get
		//our own global transform.
		if (m_parent)
			m_global = (*m_owner)[m_parentInd].m_global * local;
		else
			m_global = local;

		//Repeat the process for our child joints.
		for (auto childInd : m_childrenInd)
		{
			(*m_owner)[childInd].DoFK();
		}
	}

	Skeleton::Skeleton()
	{
		m_rootInd = 0;
	}

	//To copy a skeleton.
	Skeleton& Skeleton::operator=(const Skeleton& other)
	{
		if (&other != this)
		{
			m_joints.resize(other.m_joints.size());

			for (size_t i = 0; i < m_joints.size(); ++i)
			{
				m_joints[i] = Joint(this, other.m_joints[i]);
			}

			m_rootInd = other.m_rootInd;
		}

		return *this;
	}

	//Forward kinematics.
	void Skeleton::DoFK()
	{
		//Since FK is recursive, we just call it on the root joint.
		if (m_rootInd < m_joints.size())
			m_joints[m_rootInd].DoFK();
	}

	Joint& Skeleton::operator[](int index)
	{
		return m_joints[index];
	}

	void SkinnedMesh::SetJointInfluences(const std::vector<glm::vec4>& jointInfluences)
	{
		m_jointInfluences = jointInfluences;
		SetVBO(Attrib::JOINT_INFLUENCE, 4, jointInfluences);
	}

	void SkinnedMesh::SetSkinWeights(const std::vector<glm::vec4>& skinWeights)
	{
		m_skinWeights = skinWeights;
		SetVBO(Attrib::SKIN_WEIGHT, 4, skinWeights);
	}
}