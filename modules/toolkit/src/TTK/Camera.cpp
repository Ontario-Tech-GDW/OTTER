//////////////////////////////////////////////////////////////////////////
//
// This file is a part of the Tutorial Tool Kit (TTK) library. 
// You may not use this file in your GDW games.
//
// This file implements the TTK simple first person camera
//
// Based off of TTK by Michael Gharbharan 2017
// Shawn Matthews 2019
//
//////////////////////////////////////////////////////////////////////////
#include "TTK/Camera.h"
#include "Logging.h"

void TTK::Camera::update() {
	ViewMatrix = glm::lookAt(cameraPosition, cameraPosition + forwardVector, upVector);
}

void TTK::Camera::processMouseMotion(double newX, double newY, double prevX, double prevY, float dt)
{
	float changeX = static_cast<float>(newX - prevX);
	float changeY = static_cast<float>(newY - prevY);
	
	if ((abs(changeX) >= 200.0f) || (abs(changeY) >= 200.0f)) // hack to prevent jumps
		return;

	float pitch = (0.08f * changeY);
	float yaw = (0.08f * changeX);

	// yaw rotation
	forwardVector = glm::normalize(forwardVector);
	forwardVector = glm::rotate(forwardVector, glm::radians(yaw), upVector);

	// recalculate right vector
	rightVector = glm::cross(forwardVector, upVector);
	rightVector = glm::normalize(rightVector);

	// pitch rotation
	forwardVector = glm::normalize(glm::rotate(forwardVector, glm::radians(pitch), rightVector));

	// calc up vector
	//upVector = glm::cross(rightVector, forwardVector);
	upVector = glm::normalize(upVector);
}
