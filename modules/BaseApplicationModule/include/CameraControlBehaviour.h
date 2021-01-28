#pragma once
#include "IBehaviour.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

class CameraControlBehaviour : public IBehaviour
{
public:
	void OnLoad(entt::handle entity) override;
	void Update(entt::handle entity) override;

protected:
	float _moveSpeed = 1.5f;
	double _prevMouseX, _prevMouseY;
	float _rotationX, _rotationY;
	bool _isPressed;
	glm::quat _initial;
};
