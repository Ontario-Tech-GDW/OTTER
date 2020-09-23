/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Spritesheet.h
Simple utilities for managing spritesheets/sprite animations.
*/

#pragma once

#include "NOU/Texture.h"
#include "GLM/glm.hpp"

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace nou
{
	class Spritesheet
	{
		public:

		//Order for vertex specification in frame data.
		enum VertIndex
		{
			BOTTOM_LEFT = 0,
			BOTTOM_RIGHT,
			TOP_RIGHT,
			TOP_LEFT
		};

		struct Frame
		{
			glm::vec2 uv[4];
		};

		struct Animation
		{
			std::vector<Frame> frames;
			float frameTime;
		};

		Spritesheet(const Texture2D& tex, const glm::vec2& frameSize);
		~Spritesheet() = default;

		//Adds a new animation with given name from start frame to end frame inclusive.
		bool AddAnimation(const std::string& name, size_t startFrame, size_t endFrame, float fps);
		const Animation* GetAnimation(const std::string& name) const;

		const glm::vec2& GetFrameSize() const;

		//Specify and retrieve a default frame for the spritesheet (e.g., a "rest" pose).
		void SetDefaultFrame(int frame);
		const Frame& GetDefaultFrame() const;
		
		protected:

		std::map<std::string, std::unique_ptr<Animation>> m_anim;

		//Typedef is just to make inserting into our animation map a bit cleaner-looking.
		typedef std::map<std::string, std::unique_ptr<Animation>>::value_type NamedAnimation;

		Frame m_defaultFrame;

		glm::vec2 m_frameSize;
		glm::vec2 m_uvFrameSize;
		int m_rows, m_cols;
	};
}