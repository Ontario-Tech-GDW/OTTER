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
	}

	SkeletalAnimClip::JointPose::JointPose()
	{
		pos = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	SkeletalAnimClip::SkeletalAnimClip(const SkeletalAnim& anim, const Skeleton& skeleton)
		: m_anim(anim)
	{
		m_timer = 0.0f;

		//Should have output for each joint in our skeleton.
		m_result.resize(skeleton.m_joints.size());

		for (size_t i = 0; i < skeleton.m_joints.size(); ++i)
		{
			//Output should default to the base pose of our skeleton.
			m_result[i].pos = skeleton.m_joints[i].m_basePos;
			m_result[i].rotation = skeleton.m_joints[i].m_baseRotation;
		}

		m_rotFrame.resize(m_anim.data.size());
		std::fill(m_rotFrame.begin(), m_rotFrame.end(), 0);

		m_posFrame.resize(m_anim.data.size());
		std::fill(m_posFrame.begin(), m_posFrame.end(), 0);
	}

	void SkeletalAnimClip::Update(float deltaTime, const Skeleton& skeleton)
	{
		//Update our timer.
		m_timer += deltaTime;

		//Have we passed the end of our animation?
		if (m_timer > m_anim.duration)
		{
			//Preserve "over time".
			//(e.g., If m_timer is 2.5 seconds and our duration is 2 seconds,
			//this will keep 0.5 seconds in m_timer.)
			m_timer = fmod(m_timer, m_anim.duration);

			//Reset all of our frame indices to 0 (start from beginning).
			//For this exercise, we only worry about rotation.
			std::fill(m_rotFrame.begin(), m_rotFrame.end(), 0);
		}

		for (size_t i = 0; i < m_anim.data.size(); ++i)
		{
			JointPose& pose = m_result[m_anim.data[i].jointInd];

			//Just shorthand to make our code easier to read.
			//m_anim.data[i] contains keyframe and timing information
			//for our current joint.
			const JointAnim& anim = m_anim.data[i];

			//Placeholder: Set our position to the first position keyframe.
			//(If we have position keyframes).
			//This is left as-is for this exercise (rotation only).
			if (anim.posFrames > 0)
			{
				pose.pos = m_anim.data[i].posKeys[0];
			}

			//We can only do something if we have at least 1 keyframe.	
			//If we have only one rotation keyframe, just use that.
			if (anim.rotFrames == 1)
			{
				pose.rotation = anim.rotKeys[0];
			}
			//If we have multiple keyframes, it's interpolation time.
			else if (anim.rotFrames > 1)
			{
				//We use m_rotFrame to track the index of our "current" keyframe (p0).
				size_t curFrame = m_rotFrame[i];
				//This is the index of our "next" keyframe (p1).
				size_t nextFrame = curFrame + static_cast<size_t>(1);

				//If we've passed the timestamp of p1,
				//we need to move forward.
				//(The extra index check is in case of a precision error.)
				while (m_timer > anim.rotTimes[nextFrame]
					&& nextFrame < anim.rotFrames - static_cast<size_t>(1))
				{
					++m_rotFrame[i];
					++curFrame;
					++nextFrame;
				}

				//Calculate t using inverse LERP.
				float t = (m_timer - anim.rotTimes[curFrame])
					/ (anim.rotTimes[nextFrame] - anim.rotTimes[curFrame]);

				//SLERP our keyframes to get our current rotation quaternion.
				pose.rotation = glm::mix(anim.rotKeys[curFrame], anim.rotKeys[nextFrame], t);
			}
		}
	}

	void SkeletalAnimClip::Apply(Skeleton& skeleton)
	{
		//Indices of output match joint indices.
		for (size_t i = 0; i < m_result.size(); ++i)
		{
			Joint& joint = skeleton.m_joints[i];
			joint.m_pos = m_result[i].pos;
			joint.m_rotation = m_result[i].rotation;
		}
	}
}