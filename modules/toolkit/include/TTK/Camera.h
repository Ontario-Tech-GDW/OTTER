//////////////////////////////////////////////////////////////////////////
//
// This header is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this header in your GDW games.
//
// This is a simple first person camera class
//
// Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/vec3.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/rotate_vector.hpp>
#include "GraphicsUtils.h"

namespace TTK
{
	class Camera
	{
	public:
		/*
		 * Creates a new TTK camera. To change the alignment of the world, you can either specify
		 * an align mode at creation, or change the upVector after creation
		 * @param mode The world alignment mode to start the camera with (default is Y-up)
		 */
		Camera(AlignMode mode = AlignMode::YUp) :
		    cameraPosition(10.0f),
			forwardVector(-10.0f),
			movementScalar(0.5f),
			upVector(mode == AlignMode::ZUp ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0)),
			ViewMatrix(glm::mat4(1.0f)) {
			processMouseMotion(0.0f, 0.0f, 1.0f, 1.0f, 0.03f);
		}

		// Applies view matrix to OpenGL
		void update();

		void processMouseMotion(double newX, double newY, double prevX, double prevY, float dt);

		inline void moveUp() { cameraPosition += upVector * movementScalar; }
		inline void moveDown() { cameraPosition -= upVector * movementScalar; }
		inline void moveForward() { cameraPosition += (forwardVector * movementScalar); }
		inline void moveBackward() { cameraPosition -= (forwardVector * movementScalar); }
		inline  void moveLeft() { cameraPosition -= (rightVector * movementScalar); }
		inline void moveRight() { cameraPosition += (rightVector * movementScalar); }

		glm::vec3 cameraPosition;
		glm::vec3 forwardVector;
		glm::vec3 rightVector;
		glm::vec3 upVector;
		float movementScalar;

		glm::mat4 ViewMatrix;
	};
}
