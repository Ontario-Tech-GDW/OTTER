#pragma once
#include "Movement.h"

class Puck
{
	public:

	Puck();
	Puck(Transform::sptr& t, float m);
	~Puck();

	void movePuck(float dt);

	void movePuck();
	void respawnPuck(playerTag p);
	void setTransform(Transform::sptr& t) { transform = t; };
	void setForce(vec3 f) { force = f; };
	void setDt(float d) { deltaTime = d; };
	void applyForce(vec3 f) { force += f; };
	glm::vec3 getVelocity() { return initialVelocity; };
	glm::vec3 getForce() { return force; };
	glm::vec3 getPosition() { return transform->GetLocalPosition(); };
	void setVelocity(glm::vec3 v){ initialVelocity = v; };

	private:

	//vec2 for the force of the object, initalized to zero along the x and y
	fvec3 force = vec3(0.0f, 0.0f, 0.0f);

	//Velocity
	fvec3 initialVelocity = vec3(0.0f, 0.0f, 0.0f);

	//float variable that initializes the mass of the puck
	float mass = 5.0f;
	float deltaTime = 0;

	vec3 newVelocity = vec3(0.0f, 0.0f, 0.0f);

	//friction variable 
	const float friction = 0.98f;
	vec3 initialAcceleration = vec3(0, 0, 0);
	vec3 Magnitude = vec3(0, 0, 0);
	Transform::sptr transform;
};

