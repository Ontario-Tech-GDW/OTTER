//////////////////////////////////////////////////////////////////////////
//
// This header is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this header in your GDW games.
// 
// This class is a helper for drawing an animated sprite from a sprite sheet
//
// Michael Gharbharan 2015 - 2017
// Shawn Matthews - 2019
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <GLM/glm.hpp>
#include "Texture2D.h"
#include <vector>

namespace TTK {

	/*
	 * Represents the coordinates that a single sprite in a sheet occupies,
	 * in both pixel and normalized coordinates
	 */
	struct SpriteCoordinates
	{
	public:
		float xMin, xMax, yMin, yMax; // in pixels
		float uMin, uMax, vMin, vMax; // normalized texture coordinates
	};

	class SpriteSheetQuad
	{
	public:
		/*
		 * Creates a new empty sprite sheet, initialize it with SliceSpriteSheet
		 */
		SpriteSheetQuad();

		/*
		 * Calculates coordinates for each sprite in sheet
		 * @param fileName The path to the texture to load, relative to the current working directory
		 * @param spriteSizeX The width of each sprite in the sheet
		 * @param spriteSizeY The height of each sprite in the sheet
		 * @param numSpritesPerRow The number of sprites in a single row
		 * @param numRows The number of rows that make up the sheet
		 * @animTim The time it should take to complete one full cycle of the animation, if this is 0, then the sprite will default to 60 FPS
		 */
		[[deprecated("Please use the overload that does not take explicit sizes for sprites, as these can be calculated automatically")]]
		void SliceSpriteSheet(const char* fileName, float spriteSizeX, float spriteSizeY,
			int numSpritesPerRow, int numRows, float animTime = 0.0f);
		/*
		 * Calculates coordinates for each sprite in sheet
		 * @param fileName The path to the texture to load, relative to the current working directory
		 * @param numSpritesPerRow The number of sprites in a single row
		 * @param numRows The number of rows that make up the sheet
		 * @animTim The time it should take to complete one full cycle of the animation, if this is 0, then the sprite will default to 60 FPS
		 */
		void SliceSpriteSheet(const char* fileName, int numSpritesPerRow, int numRows, float animTime = 0.0f);

		/*
		 * Updates this sprite, and advances to the next frame if required
		 * @param deltaTime The time since the last frame, in seconds
		 */
		void Update(float deltaTime);

		/*
		 * Resets the animation of this sprite sheet, setting it's frame to the first frame, and resetting it's frame timer
		 */
		void ResetAnimation();
		/*
		 * Enables or disables looping this animation
		 * @param loop True if the sprite should loop, false if otherwise
		 */
		void SetLooping(bool loop);

		/*
		 * Renders this sprite with the given transformation matrix. Note that this matrix
		 * should transform the sprite directly into clip space
		 * @param matrix The MVP matrix to render this sprite with
		 */
		void Draw(const glm::mat4& matrix);

		/*
		 * Sets a given frame to last for a given duration in seconds
		 * @param frameNumber The index of the Frame
		 * @param time The time this frame should be presented for, in seconds
		 */
		void SetFrameLength(int frameNumber, float time);

		/*
		 * Overrides all frame lengths with a given vector of values, note that the given
		 * vector's size must match the number of frames in the animation exactly
		 */
		void SetFrameLengths(const std::vector<float>& time);

		/*
		 * Gets the vector containing all frame lengths
		 * @returns A vector containing the length of each frame
		 */
		const std::vector<float>& GetFrameLengths() const;
		/*
		 * Gets the length of a given frame in the animation
		 * @param frameNumber The index of the frame to get the time for
		 * @returns The duration that the given frame is shown for, in seconds
		 */
		float GetFrameLength(int frameNumber) const;

		/*
		 * Gets the number of frames in this animation
		 */
		int GetNumberOfFrames() const;

	private:
		struct QuadVert {
			glm::vec3 Position;
			glm::vec2 Texture;
		};

		int   m_CurrentFrame;
		float m_FrameTime;
		bool  m_DoesLoop;
		Texture2D m_Texture;
		glm::vec4 m_Color;
		QuadVert  m_Vertices[4];
		uint32_t m_VAO, m_VBO, m_EBO, m_Shader;

		std::vector<SpriteCoordinates> m_SpriteCoordinates;

		// Displays the amount of time each frame is visible for
		std::vector<float> m_FrameLength;
	};

}
