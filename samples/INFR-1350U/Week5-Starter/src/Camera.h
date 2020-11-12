#pragma once

#include <memory>
#include <GLM/glm.hpp>

/// <summary>
/// Represents a simple perspective camera for use by first person or third person games
/// </summary>
class Camera
{
public:
	typedef std::shared_ptr<Camera> sptr;
	inline static sptr Create() {
		return std::make_shared<Camera>();
	}

public:
	Camera();
	virtual ~Camera() = default;

	/// <summary>
	/// Sets this camera's position in world space
	/// </summary>
	void SetPosition(const glm::vec3& position);
	/// <summary>
	/// Sets the camera's forward vector, ie what way it's facing
	/// </summary>
	void SetForward(const glm::vec3& forward);
	/// <summary>
	/// Sets the camera's up vector, this can be thought of as a vector sticking out of the top of the camera's head
	/// </summary>
	void SetUp(const glm::vec3& up);
	/// <summary>
	/// Makes this camera look at the given point in world coordinates
	/// </summary>
	void LookAt(const glm::vec3& point);

	/// <summary>
	/// Notifies this camera that the window has resized, and updates our projection matrix
	/// </summary>
	/// <param name="windowWidth">The new width of the window</param>
	/// <param name="windowHeight">The new height of the window</param>
	void ResizeWindow(int windowWidth, int windowHeight);
	/// <summary>
	/// Sets the vertical field of view in radians for this camera
	/// </summary>
	void SetFovRadians(float value);
	/// <summary>
	/// Sets the vertical field of view in degrees for this camera
	/// </summary>
	void SetFovDegrees(float value);


	//Set's the orthographiv view for the camera
	void SetOrthoView(bool ortho);


	/// <summary>
	/// Gets the camera's position in world space
	/// </summary>
	const glm::vec3& GetPosition() const { return _position; }
	/// <summary>
	/// Gets the direction that this camera is facing in world space
	/// </summary>
	const glm::vec3& GetForward() const { return _normal; }
	/// <summary>
	/// Gets the up direction for this camera (basically a vector pointing out the top of the camera's head)
	/// </summary>
	const glm::vec3& GetUp() const { return _up; }

	float GetFovDegrees() const { return glm::degrees(_fovRadians); }
	
	/// <summary>
	/// Gets the view matrix for this camera
	/// </summary>
	const glm::mat4& GetView() const { return _view; }
	/// <summary>
	/// Gets the projection matrix for this camera
	/// </summary>
	const glm::mat4& GetProjection() const { return _projection; }
	/// <summary>
	/// Gets the combined view-projection matrix for this camera, calculating if needed
	/// </summary>
	const glm::mat4& GetViewProjection() const;


	//Get the orthographic view for the camera
	bool GetOrthoView();

protected:
	float _nearPlane;
	float _farPlane;
	
	//Created variables for the orthographic view
	float _left;
	float _right;
	float _top;
	float _bottom;

	float _fovRadians;
	float _aspectRatio;
	bool isOrtho;


	glm::vec3 _position;
	glm::vec3 _normal;
	glm::vec3 _up;

	glm::mat4 _view;
	glm::mat4 _projection;

	// The view projection, it is mutable so we can re-calculate it during const methods
	mutable glm::mat4 _viewProjection;
	// A dirty flag that indicates whether we need to re-calculate our view projection matrix
	mutable bool      _isDirty;



	


	// Recalculates the projection matrix
	void __CalculateProjection();
	// Recalculates the view matrix
	void __CalculateView();
};