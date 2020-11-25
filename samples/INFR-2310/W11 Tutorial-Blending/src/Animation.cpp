/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Animation.cpp
Classes for managing animation data and blendtrees.
*/

#include "Animation.h"

namespace nou
{
	JointAnim::JointAnim()
	{
		jointInd = 0;
		rotFrames = 0;
		posFrames = 0;
	}

	SkeletalAnim::SkeletalAnim()
	{
		duration = 0.0f;
		isDiffClip = false;
	}

	void SkeletalAnim::Keep(const std::vector<int> joints)
	{
		auto it = data.begin();

		while(it != data.end())
		{
			//Discard any data that is on a joint *not* in our list.
			if (std::find(joints.begin(), joints.end(), it->jointInd) == joints.end())
			{
				it = data.erase(it);
				continue;
			}
				
			++it;
		}
	}

	void SkeletalAnim::MakeDiffWith(const Skeleton& skeleton)
	{
		isDiffClip = true;

		for (size_t i = 0; i < data.size(); ++i)
		{
			for (size_t j = 0; j < data[i].posFrames; ++j)
			{
				//Diff position = position - base position.
				data[i].posKeys[j] = data[i].posKeys[j] - skeleton.m_joints[data[i].jointInd].m_basePos;
			}

			for (size_t j = 0; j < data[i].rotFrames; ++j)
			{
				//Diff rotation = rotation * inverse of base rotation.
				data[i].rotKeys[j] = data[i].rotKeys[j] * glm::inverse(skeleton.m_joints[data[i].jointInd].m_baseRotation);
			}
		}
	}

	SkeletalAnimNode::JointPose::JointPose()
	{
		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	SkeletalAnimNode::SkeletalAnimNode(const SkeletalAnim& anim, const Skeleton& skeleton)
		: m_anim(anim)
	{
		m_timer = 0.0f;

		m_rhs = nullptr;
		m_mode = BlendMode::PASS;
		m_blendParam = 0.0f;

		//Should have output for each joint in our skeleton.
		m_output.resize(skeleton.m_joints.size());
		m_lhs.resize(skeleton.m_joints.size());

		glm::vec3 zeroPos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat zeroRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		for (size_t i = 0; i < skeleton.m_joints.size(); ++i)
		{
			//Output should default to the base pose of our skeleton.
			m_output[i].pos = skeleton.m_joints[i].m_basePos;
			m_output[i].rotation = skeleton.m_joints[i].m_baseRotation;

			//Local output will depend on whether or not we're a diff clip.
			//If we are a diff clip, transforms should be identity.
			//If we're not a diff clip, transforms should be base pose.
			m_lhs[i].pos = (anim.isDiffClip) ? zeroPos : skeleton.m_joints[i].m_basePos;
			m_lhs[i].rotation = (anim.isDiffClip) ? zeroRot : skeleton.m_joints[i].m_baseRotation;
		}

		m_rotFrame.resize(m_anim.data.size());
		std::fill(m_rotFrame.begin(), m_rotFrame.end(), 0);

		m_posFrame.resize(m_anim.data.size());
		std::fill(m_posFrame.begin(), m_posFrame.end(), 0);
	}

	void SkeletalAnimNode::SetRHS(SkeletalAnimNode* rhs, 
						          SkeletalAnimNode::BlendMode mode,
								  float blendParam)
	{
		m_rhs = rhs;
		m_mode = mode;
		m_blendParam = blendParam;
	}

	void SkeletalAnimNode::SetBlendParam(float blendParam)
	{
		m_blendParam = blendParam;
	}

	void SkeletalAnimNode::Update(float deltaTime, const Skeleton& skeleton)
	{
		//First, update our RHS node if we have one.
		//(Assuming we care about it - i.e., we are not in the PASS mode).
		if(m_rhs != nullptr && m_mode != BlendMode::PASS)
			m_rhs->Update(deltaTime, skeleton);

		//Update our timer - if we have a time to work off of.
		if (m_anim.duration != 0.0f)
		{
			m_timer += deltaTime;

			//Should we loop over to the beginning?
			if (m_timer > m_anim.duration)
			{
				std::fill(m_rotFrame.begin(), m_rotFrame.end(), 0);
				std::fill(m_posFrame.begin(), m_posFrame.end(), 0);
			}

			while (m_timer > m_anim.duration)
				m_timer -= m_anim.duration;
		}

		//Interpolate joint rotations.
		UpdateRotations();
		//Interpolate joint positions.
		UpdatePositions();
		//Set the output of this node.
		UpdateOutput(skeleton);
	}

	void SkeletalAnimNode::Apply(Skeleton& skeleton)
	{
		//Indices of output match joint indices.
		for (size_t i = 0; i < m_output.size(); ++i)
		{
			Joint& joint = skeleton.m_joints[i];
			joint.m_pos = m_output[i].pos;
			joint.m_rotation = m_output[i].rotation;
		}
	}

	const std::vector<SkeletalAnimNode::JointPose>& SkeletalAnimNode::GetOutput()
	{
		return m_output;
	}

	void SkeletalAnimNode::UpdatePositions()
	{
		for (size_t i = 0; i < m_anim.data.size(); ++i)
		{
			//If we have no position data for this joint, move on.
			if (m_anim.data[i].posFrames == 0)
				continue;

			JointPose& pose = m_lhs[m_anim.data[i].jointInd];

			//If we only have one frame, use that.
			if (m_anim.data[i].posFrames == 1)
			{
				pose.pos = m_anim.data[i].posKeys[0];
				continue;
			}

			//Update our frame index.
			//We stop at frames - 2 since we make the assumption
			//that our animation's last frame is the same as its
			//first (a perfect loop) - this may or may not
			//be the case depending on how you manage your own animation work.
			while (m_timer > m_anim.data[i].posTimes[m_posFrame[i]
				+ static_cast<size_t>(1)]
				&& m_posFrame[i] < m_anim.data[i].posFrames - 2)
			{
				++m_posFrame[i];
			}

			size_t curFrame = m_posFrame[i];
			size_t nextFrame = m_posFrame[i] + static_cast<size_t>(1);

			float start = m_anim.data[i].posTimes[curFrame];
			float end = m_anim.data[i].posTimes[nextFrame];

			float t = (m_timer - start) / (end - start);

			if (t < 0.0f)
				t = 0.0f;

			else if (t > 1.0f)
				t = 1.0f;

			//For vectors, glm::mix does LERP.
			pose.pos = glm::mix(m_anim.data[i].posKeys[curFrame],
								m_anim.data[i].posKeys[nextFrame],
								t);
		}
	}

	void SkeletalAnimNode::UpdateRotations()
	{
		for (size_t i = 0; i < m_anim.data.size(); ++i)
		{
			if (m_anim.data[i].rotFrames == 0)
				continue;

			JointPose& pose = m_lhs[m_anim.data[i].jointInd];

			if (m_anim.data[i].rotFrames == 1)
			{
				pose.rotation = m_anim.data[i].rotKeys[0];
				continue;
			}

			//Update our frame index.
			//We stop at frames - 2 since we make the assumption
			//that our animation's last frame is the same as its
			//first (a perfect loop) - this may or may not
			//be the case depending on how you manage your own animation work.
			while (m_timer > m_anim.data[i].rotTimes[m_rotFrame[i]
				+ static_cast<size_t>(1)]
				&& m_rotFrame[i] < m_anim.data[i].rotFrames - 2)
			{
				++m_rotFrame[i];
			}

			size_t curFrame = m_rotFrame[i];
			size_t nextFrame = m_rotFrame[i] + static_cast<size_t>(1);

			float start = m_anim.data[i].rotTimes[curFrame];
			float end = m_anim.data[i].rotTimes[nextFrame];

			float t = (m_timer - start) / (end - start);

			if (t < 0.0f)
				t = 0.0f;

			else if (t > 1.0f)
				t = 1.0f;
	
			//For quaternions, glm::mix does SLERP.
			pose.rotation = glm::mix(m_anim.data[i].rotKeys[curFrame],
									 m_anim.data[i].rotKeys[nextFrame],
									 t);
		}
	}

	void SkeletalAnimNode::UpdateOutput(const Skeleton& skeleton)
	{
		//If we don't have an RHS node, or we don't care about it 
		//- copy our own data to the output of blending.
		if (m_rhs == nullptr || m_mode == BlendMode::PASS)
		{
			for (size_t i = 0; i < m_lhs.size(); ++i)
			{
				//If we're a diff clip, stack the result of our animation
				//with the skeleton's base transform.
				//Otherwise, just copy the LHS data outright.
				m_output[i].pos = (m_anim.isDiffClip) 
					? skeleton.m_joints[i].m_basePos + m_lhs[i].pos
					: m_lhs[i].pos;

				m_output[i].rotation = (m_anim.isDiffClip)
					? m_lhs[i].rotation * skeleton.m_joints[i].m_baseRotation
					: m_lhs[i].rotation;
			}

			return;
		}

		const std::vector<JointPose>& rhs_output = m_rhs->GetOutput();

		if(rhs_output.size() != m_output.size())
			return;

		switch (m_mode)
		{
			case BlendMode::BLEND:

			//TODO: Implement this blend mode.

			break;

			case BlendMode::ADD:

			//Additive blending - "stack" our transform on the
			//RHS.
			for (size_t i = 0; i < m_output.size(); ++i)
			{
				//For position, add our position to the RHS output.
				m_output[i].pos = rhs_output[i].pos + m_lhs[i].pos;
				//For rotation, multiply our rotation by the RHS output
				//(RHS rotation happens first).
				m_output[i].rotation = m_lhs[i].rotation * rhs_output[i].rotation;

				//Our output mixes between "no addition" and "full addition"
				//per the blend strength parameter.
				m_output[i].pos = glm::mix(rhs_output[i].pos,
										   m_output[i].pos,
										   m_blendParam);

				m_output[i].rotation = glm::mix(rhs_output[i].rotation,
												m_output[i].rotation,
												m_blendParam);
			}

			break;

			default:

			break;
		}
	}

	Blendtree::Blendtree(const Skeleton& skeleton) 
	{
		m_skeleton = &skeleton;
	}

	void Blendtree::Clear()
	{
		m_tree.clear();
	}

	SkeletalAnimNode* Blendtree::Insert(const SkeletalAnim& anim, SkeletalAnimNode::BlendMode mode, float blendParam)
	{
		std::unique_ptr<SkeletalAnimNode> clip = std::make_unique<SkeletalAnimNode>(anim, *m_skeleton);

		//If we're not the first node added to this tree,
		//add the existing "leaf" as the RHS of the added clip.
		if (!m_tree.empty())
			clip->SetRHS(m_tree.front().get(), mode, blendParam);

		SkeletalAnimNode* newClip = clip.get();

		//Add ourselves to the tree.
		m_tree.push_front(std::move(clip));

		//Return the node we just added (e.g., if we want to be able to change its blend weight later).
		return newClip;
	}

	void Blendtree::Update(float deltaTime, const Skeleton& skeleton)
	{
		if(m_tree.empty())
			return;

		m_tree.front()->Update(deltaTime, skeleton);
	}

	void Blendtree::Apply(Skeleton& skeleton)
	{
		if(m_tree.empty())
			return;
		
		m_tree.front()->Apply(skeleton);
	}
}