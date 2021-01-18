#include "Transform.h"

#include <GLM/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/quaternion.hpp>

#include "Logging.h"

const glm::mat4 IDENTITY = glm::mat4(1.0f);

Transform& Transform::SetLocalRotation(const glm::vec3 eulerDegrees) {
	_rotationEulerDeg = eulerDegrees;
	_rotation = glm::quat(glm::radians(eulerDegrees));
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::SetLocalRotation(const glm::quat& quaternion) {
	_rotation = quaternion;
	_rotationEulerDeg = glm::degrees(glm::eulerAngles(_rotation));
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::SetLocalRotation(float yawDeg, float pitchDeg, float rollDeg) {
	_rotationEulerDeg.x = yawDeg;
	_rotationEulerDeg.y = pitchDeg;
	_rotationEulerDeg.z = rollDeg;
	_rotation = glm::quat(glm::radians(_rotationEulerDeg));
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::SetLocalPosition(float x, float y, float z) {
	_position.x = x;
	_position.y = y;
	_position.z = z;
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::SetLocalScale(float x, float y, float z) {
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::RotateLocal(float yawDeg, float pitchDeg, float rollDeg) {
	RotateLocal(glm::vec3(yawDeg, pitchDeg, rollDeg));
	return *this;
}

Transform& Transform::RotateLocalFixed(const glm::vec3& rotationDeg) {
	_rotation = glm::quat(glm::radians(rotationDeg)) * _rotation;
	_rotationEulerDeg = glm::degrees(glm::eulerAngles(_rotation));
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::RotateLocalFixed(float yawDeg, float pitchDeg, float rollDeg) {
	RotateLocalFixed(glm::vec3(yawDeg, pitchDeg, rollDeg));
	return *this;
}

Transform& Transform::SetLocalPosition(const glm::vec3 value) {
	_position = value;
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::SetLocalScale(const glm::vec3 value) {
	_scale = value;
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::RotateLocal(const glm::vec3& rotation) {
	_rotation = _rotation * glm::quat(glm::radians(rotation));
	_rotationEulerDeg = glm::degrees(glm::eulerAngles(_rotation));
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::MoveLocal(const glm::vec3& localMovement)
{
	_position += _rotation * localMovement;
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::MoveLocal(float x, float y, float z) {
	MoveLocal(glm::vec3(x, y, z));
	return *this;
}

Transform& Transform::MoveLocalFixed(const glm::vec3& localMovement)
{
	_position += localMovement;
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::MoveLocalFixed(float x, float y, float z) {
	_position.x += x;
	_position.y += y;
	_position.z += z;
	_isLocalDirty = true;
	return *this;
}

Transform& Transform::LookAt(const glm::vec3& localSpace)
{
	_rotation = glm::quatLookAt(-glm::normalize(_position - localSpace), glm::normalize(_rotation * glm::vec3(0, 0, 1)));
	_rotationEulerDeg = glm::degrees(glm::eulerAngles(_rotation));
	_isLocalDirty = true;
	return *this;
}

void Transform::Recalculate() const {
	_UpdateLocalTransformIfDirty();
}

const glm::mat4& Transform::LocalTransform() const {
	_UpdateLocalTransformIfDirty();
	return _localTransform;
}

const glm::mat3& Transform::NormalMatrix() const {
	_UpdateLocalTransformIfDirty();
	return _normalMatrix;	
}

void Transform::SetParent(entt::handle parent)
{
	_parent = parent;
	// If we passed in a handle, make sure it has a transform and belongs to the same scene
	if (&parent.registry() != nullptr && parent.entity() != entt::null) {
		LOG_ASSERT(parent.has<Transform>(), "Parent entity must have a transform component");
		LOG_ASSERT(&parent.registry() == &_gameObject.registry(), "Parent entity must be in same registry!");
		_hierarchyDepth = parent.get<Transform>()._hierarchyDepth + 1;
	} else {
		_hierarchyDepth = 0;
	}
	
	// Re-calculate hierarchy depth for all children recursively
	_gameObject.registry().view<Transform>().each([&](entt::entity entity, Transform& t) {
		if (t._parent == _gameObject) {
			t.SetParent(entt::handle(parent.registry(), _gameObject));
		}
	});
	// Re-sort components
	_gameObject.registry().sort<Transform>([](const Transform& l, const Transform& r) {
		return l.GetHierarchyDepth() < r.GetHierarchyDepth();
	});
}

void Transform::UpdateWorldMatrix() const {
	if (_parent != entt::null) {
		_worldTransform = _gameObject.registry().get<Transform>(_parent)._worldTransform * LocalTransform();
		_worldNormalMatrix = glm::mat3(glm::transpose(glm::inverse(_worldTransform)));
	} else {
		_worldTransform = LocalTransform();
		_worldNormalMatrix = _normalMatrix;
	}
}

void Transform::_UpdateLocalTransformIfDirty() const {
	if (_isLocalDirty) {
		// TRS
		_localTransform = glm::translate(IDENTITY, _position) * glm::toMat4(_rotation) * glm::scale(IDENTITY, _scale);
		_normalMatrix = glm::mat3(glm::transpose(glm::inverse(_localTransform)));

		_isLocalDirty = false;
	}
}
