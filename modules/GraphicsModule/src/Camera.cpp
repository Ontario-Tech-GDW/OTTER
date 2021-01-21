#include "Camera.h"

#include <GLM/gtc/matrix_transform.hpp>

Camera::Camera() :
    _isOrtho(false),
	_orthoHeight(1.0f),
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

void Camera::SetIsOrtho(bool isOrtho) {
	_isOrtho = isOrtho;
	__CalculateProjection();
}

void Camera::SetOrthoHeight(float orthoHeight) {
	_orthoHeight = orthoHeight;
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
	if (windowWidth * windowHeight > 0) {
		_aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
		__CalculateProjection();
	}
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
		_viewProjectionNoTranslate = _projection * glm::mat4(glm::mat3(_view));
		_isDirty = false;
	}
	return _viewProjection;
}

const glm::mat4& Camera::GetViewProjNoTranslation() const
{
	if (_isDirty) {
		_viewProjection = _projection * _view;
		_viewProjectionNoTranslate = _projection * glm::mat4(glm::mat3(_view));
		_isDirty = false;
	}
	return _viewProjectionNoTranslate;
}

void Camera::__CalculateProjection() {
	if (_isOrtho) {
		_projection = glm::ortho(
			-_orthoHeight * _aspectRatio, _orthoHeight * _aspectRatio, 
			-_orthoHeight, _orthoHeight, 
			_nearPlane, _farPlane);
	}
	else {
		_projection = glm::perspective(_fovRadians, _aspectRatio, _nearPlane, _farPlane);
	}
	_isDirty = true;
}

void Camera::__CalculateView() {
	_view = glm::lookAt(_position, _position + _normal, _up);
	_isDirty = true;
}
