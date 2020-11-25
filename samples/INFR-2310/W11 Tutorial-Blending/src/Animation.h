/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Animation.h
Classes for managing animation data and blendtrees.
*/

#pragma once

#include "SkinnedMesh.h"

#include "GLM/glm.hpp"

#include <vector>
#include <deque>

namespace nou
{	
	//Store the data for an animation clip for one joint.
	struct JointAnim
	{
		int jointInd;

		int rotFrames;
		std::vector<float> rotTimes;
		std::vector<glm::quat> rotKeys;

		int posFrames;
		std::vector<float> posTimes;
		std::vector<glm::vec3> posKeys;

		JointAnim();
	};

	//Store the data for an animation clip for an entire skeleton.
	struct SkeletalAnim
	{
		float duration;
		std::vector<JointAnim> data;
		bool isDiffClip;

		SkeletalAnim();

		//Discard all animation data except for the joint indices
		//contained in joints.
		void Keep(const std::vector<int> joints);

		//Turn this animation into a diff clip with the base pose
		//of the given skeleton.
		void MakeDiffWith(const Skeleton& skeleton);
	};

	//Manage an animation clip.
	//Treated as a "node" in our blend tree - any clip
	//can have a "right-hand side" that it will blend with.
	class SkeletalAnimNode
	{
		public:

		struct JointPose
		{
			glm::vec3 pos;
			glm::quat rotation;

			JointPose();
			~JointPose() = default;
		};

		enum class BlendMode
		{
			//Don't blend at all (ignore RHS' output).
			PASS = 0,
			//Linear blend with RHS' output.
			BLEND,
			//Additive blend to RHS (add ourselves to RHS' output).
			ADD
		};

		SkeletalAnimNode(const SkeletalAnim& anim, const Skeleton& skeleton);
		~SkeletalAnimNode() = default;

		//Add a "right hand side" node we should blend with.
		void SetRHS(SkeletalAnimNode* rhs, BlendMode mode, float blendParam);
		//Set the strength of the blend between ourselves and the RHS node.
		void SetBlendParam(float blendParam);

		//Update this node's animation.
		void Update(float deltaTime, const Skeleton& skeleton);
		//Apply the output of this node to a skeleton.
		void Apply(Skeleton& skeleton);

		//Get the output of this node.
		const std::vector<JointPose>& GetOutput();

		protected:

		//A pointer to the RHS node (if we have one).
		SkeletalAnimNode* m_rhs;
		//Which blend mode to use.
		BlendMode m_mode;
		//The strength of the blend between ourselves and the RHS node.
		float m_blendParam;

		//Our local timer.
		float m_timer;
		//The data for our animation clip.
		const SkeletalAnim& m_anim;
		//Which frame indices we are on for each joint.
		std::vector<int> m_rotFrame;
		std::vector<int> m_posFrame;

		//The result of our own animation update.
		//(Ignoring the right-hand-side node.)
		std::vector<JointPose> m_lhs;
		//The output of blending between our own animation
		//and our RHS node.
		std::vector<JointPose> m_output;

		void UpdateRotations();
		void UpdatePositions();
		void UpdateOutput(const Skeleton& skeleton);
	};

	class Blendtree
	{
		public:

		Blendtree(const Skeleton& skeleton);
		~Blendtree() = default;

		void Clear();
		//Insert a new node at the "end" of the tree.
		SkeletalAnimNode* Insert(const SkeletalAnim& anim,
								 SkeletalAnimNode::BlendMode mode = SkeletalAnimNode::BlendMode::PASS,
								 float blendParam = 0.0f);

		void Update(float deltaTime, const Skeleton& skeleton);
		void Apply(Skeleton& skeleton);

		private:

		const Skeleton* m_skeleton;
		//A chain of nodes representing our blend tree.
		//Example:
		//Root node has IDLE animation with PASS mode.
		//Next node has WALK animation with BLEND mode - blend between idle and walk.
		//Next node has NOD animation with ADD mode - add head nodding onto result of blend
		//between idle and walk.
		std::deque<std::unique_ptr<SkeletalAnimNode>> m_tree;
	};
}