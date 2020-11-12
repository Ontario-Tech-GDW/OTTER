#pragma once
#include <memory>
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

/// <summary>
/// A simple transformation class, without parent/child relationships
/// </summary>
class Transform final
{
public:
	// We'll disallow moving and copying, since we want to manually control when the destructor is called
	// We'll use these classes via pointers
	Transform(const Transform& other) = delete;
	Transform(Transform&& other) = delete;
	Transform& operator=(const Transform& other) = delete;
	Transform& operator=(Transform&& other) = delete;

	typedef std::shared_ptr<Transform> sptr;
	static inline sptr Create() {
		return std::make_shared<Transform>();
	}

public:
	Transform() :
		_isLocalDirty(true),
		_localTransform(glm::mat4(1.0f)),
		_normalMatrix(glm::mat3(1.0f)),
		_rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		_rotationEulerDeg(glm::vec3(0.0f)),
		_position(glm::vec3(0.0f)),
		_scale(glm::vec3(1.0f))
	{}
	virtual ~Transform() = default;

	// Rotation Getters/Setters

	/// <summary>
	/// Gets the local rotation of the transform in euler degrees
	/// </summary>
	const glm::vec3& GetLocalRotation() const { return _rotationEulerDeg; }
	/// <summary>
	/// Sets the local rotation of this transform to the given value in euler degrees
	/// </summary>
	/// <param name="eulerDegrees">The rotation (yaw, pitch, roll) in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* SetLocalRotation(const glm::vec3 eulerDegrees);
	/// <summary>
	/// Sets the local rotation of this transform to the given value in euler degrees
	/// </summary>
	/// <param name="yawDeg">The yaw in degrees</param>
	/// <param name="pitchDeg">The pitch in degrees</param>
	/// <param name="rollDeg">The roll in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* SetLocalRotation(float yawDeg, float pitchDeg, float rollDeg);

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
	Transform* SetLocalPosition(const glm::vec3 value);
	/// <summary>
	/// Sets this transforms translation within it's local space
	/// </summary>
	/// <param name="x">The x coordinate in local space</param>
	/// <param name="y">The y coordinate in local space</param>
	/// <param name="z">The z coordinate in local space</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* SetLocalPosition(float x, float y, float z);

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
	Transform* SetLocalScale(const glm::vec3 value);
	/// <summary>
	/// Sets this transforms scale within it's local space
	/// </summary>
	/// <param name="x">The x scaling factor in local space</param>
	/// <param name="y">The y scaling factor in local space</param>
	/// <param name="z">The z scaling factor in local space</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* SetLocalScale(float x, float y, float z);


	// Manipulators

	/// <summary>
	/// Applies a cumulative rotation to this transform, relative to the current rotation
	/// </summary>
	/// <param name="rotationDeg">The rotation(yaw, pitch, roll) in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* RotateLocal(const glm::vec3& rotationDeg);
	/// <summary>
	/// Applies a cumulative rotation to this transform, relative to the current rotation
	/// </summary>
	/// <param name="yawDeg">The yaw in degrees</param>
	/// <param name="pitchDeg">The pitch in degrees</param>
	/// <param name="rollDeg">The roll in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* RotateLocal(float yawDeg, float pitchDeg, float rollDeg);
	/// <summary>
	/// Applies a rotation to this transform, relative to the local space origin
	/// </summary>
	/// <param name="rotationDeg">The rotation(yaw, pitch, roll) in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* RotateLocalFixed(const glm::vec3& rotationDeg);
	/// <summary>
	/// Applies a rotation to this transform, relative to the local space origin
	/// </summary>
	/// <param name="yawDeg">The yaw in degrees</param>
	/// <param name="pitchDeg">The pitch in degrees</param>
	/// <param name="rollDeg">The roll in degrees</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* RotateLocalFixed(float yawDeg, float pitchDeg, float rollDeg);
	/// <summary>
	/// Moves the transform within local space
	/// </summary>
	/// <param name="localMovement">The translation to apply to the position in local space</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* MoveLocal(const glm::vec3& localMovement);
	/// <summary>
	/// Moves the transform within local space
	/// </summary>
	/// <param name="x">The translation along the local x axis to apply to the transformation</param>
	/// <param name="y">The translation along the local y axis to apply to the transformation</param>
	/// <param name="z">The translation along the local z axis to apply to the transformation</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* MoveLocal(float x, float y, float z);
	/// <summary>
	/// Moves the transform within local space, relative to the local space center
	/// </summary>
	/// <param name="localMovement">The translation to apply to the position in local space</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* MoveLocalFixed(const glm::vec3& localMovement);
	/// <summary>
	/// Moves the transform within local space, relative to the local space center
	/// </summary>
	/// <param name="x">The translation along the local x axis to apply to the transformation</param>
	/// <param name="y">The translation along the local y axis to apply to the transformation</param>
	/// <param name="z">The translation along the local z axis to apply to the transformation</param>
	/// <returns>A pointer to this, to allow for chaining. DO NOT STORE POINTER!</returns>
	Transform* MoveLocalFixed(float x, float y, float z);

	// Matrix gets

	/// <summary>
	/// Gets the local transformation matrix for this transform, updating it if required
	/// </summary>
	const glm::mat4& LocalTransform() const;
	/// <summary>
	/// Gets the local rotation matrix for this transform, updating it if required.
	/// This is useful for calculating the normal matrix
	/// </summary>
	const glm::mat3& NormalMatrix() const;

private:
	mutable bool _isLocalDirty;
	mutable glm::mat4 _localTransform;
	mutable glm::mat3 _normalMatrix;
	
	glm::quat _rotation;
	glm::vec3 _rotationEulerDeg;
	glm::vec3 _position;
	glm::vec3 _scale;

	void _UpdateLocalTransformIfDirty() const;
};
