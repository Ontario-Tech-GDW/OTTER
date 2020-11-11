/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

SkinnedMesh.h
Basic classes for managing the data associated with a skinned 3D model.
*/

#pragma once

#include "NOU/Mesh.h"
#include "NOU/Transform.h"

#include <vector>

namespace nou
{
	class Skeleton;

	class Joint
	{
		public:

		Joint(Skeleton* owner = nullptr);

		//We have this constructor for making a copy of
		//a skeleton - e.g., every instance
		//of a skinned mesh renderer needs its own copy
		//of skeletal data for animation.
		Joint(Skeleton* owner, const Joint& other);
		~Joint() = default;

		//Forward kinematics.
		void DoFK();

		Skeleton* m_owner;
		std::string m_name;

		//Global transformation matrix.
		glm::mat4 m_global;
		//Local position.
		glm::vec3 m_pos;
		//Local position in base pose.
		glm::vec3 m_basePos;
		//Local rotation.
		glm::quat m_rotation;
		//Local rotation in base pose.
		glm::quat m_baseRotation;
		//Inverse bind matrix.
		glm::mat4 m_invBind;

		//Do we have a parent?
		bool m_parent;
		//What is the index of our parent in the skeleton's list of joints?
		int m_parentInd;

		//What are the indices of our children in the skeleton's list of joints?
		std::vector<int> m_childrenInd;
	};

	class Skeleton
	{
		public:

		Skeleton();
		~Skeleton() = default;

		//To copy our skeleton.
		Skeleton& operator=(const Skeleton& other);

		//Forward kinematics.
		void DoFK();
		//To quickly grab a joint by its index.
		Joint& operator[](int index);

		//The root joint.
		int m_rootInd;
		//Our set of joints.
		std::vector<Joint> m_joints;
	};

	class SkinnedMesh : public Mesh
	{
		public:

		Skeleton m_skeleton;

		SkinnedMesh() = default;
		virtual ~SkinnedMesh() = default;

		void SetJointInfluences(const std::vector<glm::vec4>& jointInfluences);
		void SetSkinWeights(const std::vector<glm::vec4>& skinWeights);

		protected:

		std::vector<glm::vec4> m_jointInfluences;
		std::vector<glm::vec4> m_skinWeights;
	};
}