#pragma once
#include <entt.hpp>
#include <memory>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

/// <summary>
/// A simple transformation class, without parent/child relationships
/// </summary>
class Transform final
{
public:
	struct TransformDirtyTag { };
	
	Transform(entt::handle gameObject) :
		_isLocalDirty(true),
		_localTransform(glm::mat4(1.0f)),
		_normalMatrix(glm::mat3(1.0f)),
		_isWorldDirty(true),
		_worldTransform(glm::mat4(1.0f)),
		_worldNormalMatrix(glm::mat3(1.0f)),
		_rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		_rotationEulerDeg(glm::vec3(0.0f)),
		_position(glm::vec3(0.0f)),
		_scale(glm::vec3(1.0f)),
		_parent(entt::null),
		_gameObject(gameObject),
		_hierarchyDepth(0)
	{}
	Transform(const Transform& other) = default;
	Transform(Transform&& other) = default;
	Transform& operator =(const Transform & other) = default;
	Transform& operator =(Transform && other) = default;
	virtual ~Transform() = default;

	// Rotation Getters/Setters

	/// <summary>
	/// Gets the local rotation of the transform in euler degrees
	/// </summary>
	const glm::vec3& GetLocalRotation() const { return _rotationEulerDeg; }
	/// <summary>
	/// Returns the local rotation as a quaternion
	/// </summary>
	const glm::quat& GetLocalRotationQuat() const { return _rotation; }
	/// <summary>
	/// Sets the local rotation of this transform to the given value in euler degrees
	/// </summary>
	/// <param name="eulerDegrees">The rotation (yaw, pitch, roll) in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& SetLocalRotation(const glm::vec3 eulerDegrees);
	/// <summary>
	/// Sets the local rotation of this transform to the given value in euler degrees
	/// </summary>
	/// <param name="eulerDegrees">The rotation (yaw, pitch, roll) in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& SetLocalRotation(const glm::quat& quaternion);
	/// <summary>
	/// Sets the local rotation of this transform to the given value in euler degrees
	/// </summary>
	/// <param name="yawDeg">The yaw in degrees</param>
	/// <param name="pitchDeg">The pitch in degrees</param>
	/// <param name="rollDeg">The roll in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& SetLocalRotation(float yawDeg, float pitchDeg, float rollDeg);

	// Position Getters/Setters

	/// <summary>
	/// Gets the local position of this transform
	/// </summary>
	const glm::vec3& GetLocalPosition() const { return _position; }
	/// <summary>
	/// Sets this transforms translation within it's local space
	/// </summary>
	/// <param name="value">The transforms position in local space</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& SetLocalPosition(const glm::vec3 value);
	/// <summary>
	/// Sets this transforms translation within it's local space
	/// </summary>
	/// <param name="x">The x coordinate in local space</param>
	/// <param name="y">The y coordinate in local space</param>
	/// <param name="z">The z coordinate in local space</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& SetLocalPosition(float x, float y, float z);

	// Scale Getters/Setters

	/// <summary>
	/// Gets the local scale for this transform, along each axis
	/// </summary>
	const glm::vec3& GetLocalScale() const { return _scale; }
	/// <summary>
	/// Sets this transforms scale within it's local space
	/// </summary>
	/// <param name="value">The value of the local scaling factor for this transform</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& SetLocalScale(const glm::vec3 value);
	/// <summary>
	/// Sets this transforms scale within it's local space
	/// </summary>
	/// <param name="x">The x scaling factor in local space</param>
	/// <param name="y">The y scaling factor in local space</param>
	/// <param name="z">The z scaling factor in local space</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& SetLocalScale(float x, float y, float z);

	// Manipulators

	/// <summary>
	/// Applies a cumulative rotation to this transform, relative to the current rotation
	/// </summary>
	/// <param name="rotationDeg">The rotation(yaw, pitch, roll) in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& RotateLocal(const glm::vec3& rotationDeg);
	/// <summary>
	/// Applies a cumulative rotation to this transform, relative to the current rotation
	/// </summary>
	/// <param name="yawDeg">The yaw in degrees</param>
	/// <param name="pitchDeg">The pitch in degrees</param>
	/// <param name="rollDeg">The roll in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& RotateLocal(float yawDeg, float pitchDeg, float rollDeg);
	/// <summary>
	/// Applies a rotation to this transform, relative to the local space origin
	/// </summary>
	/// <param name="rotationDeg">The rotation(yaw, pitch, roll) in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& RotateLocalFixed(const glm::vec3& rotationDeg);
	/// <summary>
	/// Applies a rotation to this transform, relative to the local space origin
	/// </summary>
	/// <param name="yawDeg">The yaw in degrees</param>
	/// <param name="pitchDeg">The pitch in degrees</param>
	/// <param name="rollDeg">The roll in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& RotateLocalFixed(float yawDeg, float pitchDeg, float rollDeg);
	/// <summary>
	/// Moves the transform within local space
	/// </summary>
	/// <param name="localMovement">The translation to apply to the position in local space</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& MoveLocal(const glm::vec3& localMovement);
	/// <summary>
	/// Moves the transform within local space
	/// </summary>
	/// <param name="x">The translation along the local x axis to apply to the transformation</param>
	/// <param name="y">The translation along the local y axis to apply to the transformation</param>
	/// <param name="z">The translation along the local z axis to apply to the transformation</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& MoveLocal(float x, float y, float z);
	/// <summary>
	/// Moves the transform within local space, relative to the local space center
	/// </summary>
	/// <param name="localMovement">The translation to apply to the position in local space</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& MoveLocalFixed(const glm::vec3& localMovement);
	/// <summary>
	/// Moves the transform within local space, relative to the local space center
	/// </summary>
	/// <param name="x">The translation along the local x axis to apply to the transformation</param>
	/// <param name="y">The translation along the local y axis to apply to the transformation</param>
	/// <param name="z">The translation along the local z axis to apply to the transformation</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform& MoveLocalFixed(float x, float y, float z);

	/// <summary>
	/// Rotates this transform to look at the given point in local space
	/// </summary>
	/// <param name="localSpace">The point in local space to look at</param>
	Transform& LookAt(const glm::vec3& localSpace);

	// Matrix gets

	/// <summary>
	/// Forces the transform to re-calculate it's matrices
	/// </summary>
	void Recalculate() const;

	/// <summary>
	/// Gets the local transformation matrix for this transform, updating it if required
	/// </summary>
	const glm::mat4& LocalTransform() const;
	/// <summary>
	/// Gets the local rotation matrix for this transform, updating it if required.
	/// This is useful for calculating the normal matrix
	/// </summary>
	const glm::mat3& NormalMatrix() const;

	void SetParent(entt::handle parent);

	void UpdateWorldMatrix() const;

	const glm::mat4& WorldTransform() const { return _worldTransform; }
	const glm::mat3& WorldNormalMatrix() const { return _worldNormalMatrix; };

	/// <summary>
	/// Gets the depth of this transform within the scene hierarchy (ie. how many parents
	/// to the root)
	/// </summary>
	/// <returns></returns>
	int GetHierarchyDepth() const { return _hierarchyDepth; }

private:
	mutable bool _isLocalDirty;
	mutable glm::mat4 _localTransform;
	mutable glm::mat3 _normalMatrix;

	mutable bool _isWorldDirty;
	mutable glm::mat4 _worldTransform;
	mutable glm::mat3 _worldNormalMatrix;
	
	glm::quat _rotation;
	glm::vec3 _rotationEulerDeg;
	glm::vec3 _position;
	glm::vec3 _scale;

	entt::entity _parent;
	entt::handle _gameObject;
	int _hierarchyDepth;

	void _UpdateLocalTransformIfDirty() const;
};
