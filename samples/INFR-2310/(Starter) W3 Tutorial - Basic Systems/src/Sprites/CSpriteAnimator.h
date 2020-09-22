/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CSpriteAnimator.h
Simple sprite animator component.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#pragma once

#include "NOU/Entity.h"
#include "Spritesheet.h"

namespace nou
{
	class CSpriteAnimator
	{
		public:

		CSpriteAnimator(Entity& owner, Spritesheet& sheet);
		virtual ~CSpriteAnimator() = default;

		void Update(float deltaTime);

		void PlayOnce(const std::string& name);
		void PlayLoop(const std::string& name);

		//Can use to check if a one-shot animation is finished.
		//(May be useful in controlling an FSM).
		bool IsDone() const;

		protected:

		//Called by PlayOnce and PlayLoop.
		void Play(const std::string& name);

		Entity* m_owner;
		Spritesheet* m_sheet;
		const Spritesheet::Animation* m_anim;

		bool m_isPlaying;
		bool m_loop;
		bool m_isDone;

		float m_timer;
		size_t m_frame;
	};
}