#include "Camera.h"

#include <GLM/gtc/matrix_transform.hpp>

Camera::Camera() :
	_nearPlane(0.1f),
	_farPlane(1000.0f),
	_fovRadians(glm::degrees(90.0f)),
	_aspectRatio(1.0f),
	_orthoVerticalScale(1.0f),
	_isOrtho(false),
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
	_normal = glm::normalize(point - _position);
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

void Camera::SetOrthoEnabled(bool value)
{
	_isOrtho = true;
	__CalculateProjection();
}

void Camera::SetFovRadians(float value) {
	_fovRadians = value;
	__CalculateProjection();
}

void Camera::SetFovDegrees(float value) {
	SetFovRadians(glm::radians(value));
}

void Camera::SetOrthoVerticalScale(float value) {
	_orthoVerticalScale = value;
	__CalculateProjection();
}

const glm::mat4& Camera::GetViewProjection() const {
	// TODO: Calculate and return view projection if dirty, otherwise return calculated value
	return _viewProjection; // delete this
}

void Camera::__CalculateProjection()
{
	// TODO: Calculate projection matrix
}

void Camera::__CalculateView() {
	// TODO: Calculate view matrix
}
