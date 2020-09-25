/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CKnightFSM.h
Simple FSM component for our animated knight.
*/

#include "CKnightFSM.h"
#include "Sprites/CSpriteAnimator.h"

namespace nou
{
	//String labels for animations.
	const std::string CKnightFSM::idleClip = "idle";
	const std::string CKnightFSM::runClip = "walk";
	const std::string CKnightFSM::attackClip = "attack";

	CKnightFSM::CKnightFSM(Entity& owner)
	{
		m_owner = &owner;
		m_state = AnimState::IDLE;

		SetState(m_state);
	}

	void CKnightFSM::SetState(CKnightFSM::AnimState state)
	{
		m_state = state;

		//Gets the animator from the entity.
		auto& animator = m_owner->Get<CSpriteAnimator>();

		ClearTriggers();

		//Runs animation clips based off of state.
		switch (m_state)
		{
			case AnimState::IDLE:

			animator.PlayLoop(idleClip);
			break;

			case AnimState::RUN:

			animator.PlayLoop(runClip);
			break;

			//TODO (for today's task): Implement the attack state.

			default:

			break;
		}
	}


	void CKnightFSM::Update()
	{
		auto& animator = m_owner->Get<CSpriteAnimator>();

		//Perform any necessary updates and check for transitions between states.
		switch (m_state)
		{
			case AnimState::IDLE:

				if (GetVariable("moving"))
					SetState(AnimState::RUN);

				break;

			case AnimState::RUN:

				if (!GetVariable("moving"))
					SetState(AnimState::IDLE);

				break;

			//TODO (for today's task): Implement the attack state.

			default:

				break;
		}
	}	
}