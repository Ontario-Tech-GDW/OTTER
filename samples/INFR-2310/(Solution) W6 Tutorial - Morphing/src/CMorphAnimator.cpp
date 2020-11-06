/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMorphAnimator.cpp
Simple animator component for demonstrating morph target animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CMorphAnimator.h"
#include "CMorphMeshRenderer.h"
#include "NOU/Mesh.h"

namespace nou
{
	CMorphAnimator::AnimData::AnimData()
	{
		frameTime = 1.0f;
	}

	CMorphAnimator::CMorphAnimator(Entity& owner)
	{
		m_owner = &owner;
		m_data = std::make_unique<AnimData>();
		m_timer = 0.0f;
		m_frame = 0;
	}

	void CMorphAnimator::Update(float deltaTime)
	{
		//If we have no frames, this component shouldn't do anything.
		if(m_data->frames.empty())
			return;

		//If we have one frame, use it for both the current and next keyframe.
		if (m_data->frames.size() == 1)
		{
			m_owner->Get<CMorphMeshRenderer>().UpdateData(*m_data->frames[0], *m_data->frames[0], 0.0f);
			return;
		}

		float t = 0.0f;

		//If we have timing information...
		if (m_data->frameTime > 0.0f)
		{
			m_timer += deltaTime;

			//We can truncate the result of division to figure out
			//if we've skipped any frames.
			//If we've just gone over the frame time by a little bit,
			//this will give us 1.
			size_t frameSkip = m_timer / m_data->frameTime;

			//Modulus keeps us from going over the number of frames.
			m_frame = (m_frame + frameSkip) % m_data->frames.size();

			//This preserves our remainder (e.g., if we had 1.5 seconds
			//with a frame time of 1 second, this will give us 0.5 seconds).
			m_timer = fmod(m_timer, m_data->frameTime);

			//Calculate t based on the proportion of frame time elapsed.
			t = m_timer / m_data->frameTime;
		}

		size_t nextFrame = (m_frame + 1) % m_data->frames.size();

		m_owner->Get<CMorphMeshRenderer>().UpdateData(*m_data->frames[m_frame], *m_data->frames[nextFrame], t);
	}

	//Copies the data from the frames parameter into m_data->frames.
	void CMorphAnimator::SetFrames(const std::vector<std::unique_ptr<Mesh>>& frames)
	{
		m_data->frames.clear();
		m_data->frames.reserve(frames.size());

		for (size_t i = 0; i < frames.size(); ++i)
		{
			m_data->frames.push_back(frames[i].get());
		}
	}

	void CMorphAnimator::SetFrameTime(float frameTime)
	{
		m_data->frameTime = frameTime;
	}
}