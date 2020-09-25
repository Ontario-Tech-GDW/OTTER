/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Spritesheet.cpp
Simple utilities for managing spritesheets/sprite animations.
*/

#include "Spritesheet.h"

#include <cmath>

namespace nou
{
	Spritesheet::Spritesheet(const Texture2D& tex, const glm::vec2& frameSize)
	{
		int texW, texH;
		tex.GetDimensions(texW, texH);

		m_frameSize = frameSize;
		m_uvFrameSize = glm::vec2(frameSize.x / static_cast<float>(texW),
								  frameSize.y / static_cast<float>(texH));

		m_rows = static_cast<int>(std::lrint(1.0f / m_uvFrameSize.y));
		m_cols = static_cast<int>(std::lrint(1.0f / m_uvFrameSize.x));

		SetDefaultFrame(0);
	}

	bool Spritesheet::AddAnimation(const std::string& name, size_t startFrame, size_t endFrame, float fps)
	{
		std::unique_ptr<Animation> anim = std::make_unique<Animation>();

		anim->frameTime = 1.0f / fps;
		anim->frames.resize(endFrame - startFrame + 1);

		int rowIndex = static_cast<int>(startFrame) / m_cols;
		int colIndex = static_cast<int>(startFrame) % m_cols;

		float fWidth = m_uvFrameSize.x;
		float fHeight = m_uvFrameSize.y;

		//As convention, we expect that sprite frames are indexed from the top left.
		//However, by OpenGL convention, we use the bottom left of a texture as its origin.
		glm::vec2 frameOrigin = glm::vec2(static_cast<float>(colIndex) * fWidth,
										  1.0f - static_cast<float>(rowIndex + 1) * fHeight);

		for (size_t i = startFrame, f = 0; i <= endFrame; ++i, ++f)
		{
			if (rowIndex >= m_rows)
				return false;

			anim->frames[f].uv[BOTTOM_LEFT] = frameOrigin;
			anim->frames[f].uv[BOTTOM_RIGHT] = frameOrigin + glm::vec2(fWidth, 0.0f);
			anim->frames[f].uv[TOP_RIGHT] = frameOrigin + glm::vec2(fWidth, fHeight);
			anim->frames[f].uv[TOP_LEFT] = frameOrigin + glm::vec2(0.0f, fHeight);

			++colIndex;
			frameOrigin.x += static_cast<float>(fWidth);

			if (colIndex >= m_cols)
			{
				colIndex = 0;
				frameOrigin.x = 0.0f;

				++rowIndex;
				frameOrigin.y -= static_cast<float>(fHeight);
			}
		}

		m_anim.insert(NamedAnimation(name, std::move(anim)));
		return true;
	}

	const Spritesheet::Animation* Spritesheet::GetAnimation(const std::string& name) const
	{
		auto it = m_anim.find(name);

		if (it == m_anim.end())
			return nullptr;

		return it->second.get();
	}

	const glm::vec2& Spritesheet::GetFrameSize() const
	{
		return m_frameSize;
	}

	const Spritesheet::Frame& Spritesheet::GetDefaultFrame() const
	{
		return m_defaultFrame;
	}

	void Spritesheet::SetDefaultFrame(int frame)
	{
		int rowIndex = static_cast<int>(frame) / m_cols;
		int colIndex = static_cast<int>(frame) % m_cols;

		float fWidth = m_uvFrameSize.x;
		float fHeight = m_uvFrameSize.y;

		//As convention, we expect that sprite frames are indexed from the top left.
		//However, by OpenGL convention, we use the bottom left of a texture as its origin.
		glm::vec2 frameOrigin = glm::vec2(static_cast<float>(colIndex) * fWidth,
									      1.0f - static_cast<float>(rowIndex + 1) * fHeight);

		m_defaultFrame.uv[BOTTOM_LEFT] = frameOrigin;
		m_defaultFrame.uv[BOTTOM_RIGHT] = frameOrigin + glm::vec2(fWidth, 0.0f);
		m_defaultFrame.uv[TOP_RIGHT] = frameOrigin + glm::vec2(fWidth, fHeight);
		m_defaultFrame.uv[TOP_LEFT] = frameOrigin + glm::vec2(0.0f, fHeight);
	}
}