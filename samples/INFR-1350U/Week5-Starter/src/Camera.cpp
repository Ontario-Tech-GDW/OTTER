#include "Camera.h"

#include <GLM/gtc/matrix_transform.hpp>

Camera::Camera() :
<<<<<<< HEAD
	_left(-2),
	_right(2),
	_bottom(-2),
	_top(2),
=======
>>>>>>> master
	_nearPlane(0.1f),
	_farPlane(1000.0f),
	_fovRadians(glm::radians(90.0f)),
	_aspectRatio(1.0f),
	_position(glm::vec3(0.0f)),
	_normal(glm::vec3(0.0f, 0.0f, 1.0f)),
	_up(glm::vec3(0.0f, 1.0f, 0.0f)), // Using Y-up coordinate system by default
	_view(glm::mat4(1.0f)),
	_projection(glm::mat4(1.0f)),
	_viewProjection(glm::mat4(1.0f)),
	_isDirty(true)
{
	__CalculateProjection();
}

void Camera::SetPosition(const glm::vec3& position) {
	_position = position;
	__CalculateView();
}

void Camera::SetForward(const glm::vec3& forward) {
	_normal = forward;
	__CalculateView();
}

void Camera::LookAt(const glm::vec3& point) {
	_normal = -glm::normalize(_position - point);
	__CalculateView();
}

void Camera::SetUp(const glm::vec3& up) {
	_up = up;
	__CalculateView();
}

void Camera::ResizeWindow(int windowWidth, int windowHeight) {
	_aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	__CalculateProjection();
}

void Camera::SetFovRadians(float value) {
	_fovRadians = value;
	__CalculateProjection();
}

void Camera::SetFovDegrees(float value) {
	SetFovRadians(glm::radians(value));
}

const glm::mat4& Camera::GetViewProjection() const {
	if (_isDirty) {
		_viewProjection = _projection * _view;
		_isDirty = false;
	}
	return _viewProjection;
<<<<<<< HEAD
}


//Set ortho function sets the isOrtho variable to the variable passed from main
//if statments that determine if the variable is true or false.
//if isOrtho is true, set the projection to Orthographic, else set it to perspective
void Camera::SetOrthoView(bool ortho)
{
	isOrtho = ortho;
	if (isOrtho) {
		_projection = glm::ortho(_left * _aspectRatio, _right * _aspectRatio, _bottom, _top, _nearPlane, _farPlane);
		_isDirty = true;
	}
	
	else {
	_projection = glm::perspective(_fovRadians, _aspectRatio, _nearPlane, _farPlane);
	_isDirty = true;
	}

	
}

bool Camera::GetOrthoView()
{
	return isOrtho;
}


void Camera::__CalculateProjection() {
	
	_projection = glm::ortho(_left * _aspectRatio, _right * _aspectRatio, _bottom * _aspectRatio, _top * _aspectRatio, _nearPlane, _farPlane);
	_isDirty = true;
	
	_projection = glm::perspective(_fovRadians, _aspectRatio, _nearPlane, _farPlane);
	_isDirty = true;

}


=======
}

void Camera::__CalculateProjection() {
	_projection = glm::perspective(_fovRadians, _aspectRatio, _nearPlane, _farPlane);
	_isDirty = true;
}
>>>>>>> master

void Camera::__CalculateView() {
	_view = glm::lookAt(_position, _position + _normal, _up);
	_isDirty = true;
}
