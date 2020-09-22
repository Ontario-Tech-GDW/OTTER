/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CSpriteAnimator.cpp
Simple sprite animator component.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CSpriteAnimator.h"
#include "CSpriteRenderer.h"

namespace nou
{
	CSpriteAnimator::CSpriteAnimator(Entity& owner, Spritesheet& sheet)
	{
		m_owner = &owner;
		m_sheet = &sheet;

		m_anim = nullptr;
		m_isPlaying = false;
		m_isDone = true;
		m_loop = false;

		m_frame = 0;
		m_timer = 0.0f;
	}

	void CSpriteAnimator::Update(float deltaTime)
	{
		//TODO: Complete this function.
	}

	void CSpriteAnimator::PlayOnce(const std::string& name)
	{
		m_loop = false;
		Play(name);
	}

	void CSpriteAnimator::PlayLoop(const std::string& name)
	{
		m_loop = true;
		Play(name);
	}

	bool CSpriteAnimator::IsDone() const
	{
		return m_isDone;
	}

	void CSpriteAnimator::Play(const std::string& name)
	{
		m_anim = m_sheet->GetAnimation(name);

		//Only set the isPlaying flag if our animation exists.
		m_isPlaying = m_anim != nullptr;
		//If we'll be playing something, it's not done yet.
		m_isDone = !m_isPlaying;

		m_timer = 0.0f;
		m_frame = 0;	
	}
}