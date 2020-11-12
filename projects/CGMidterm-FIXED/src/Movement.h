#pragma once

#include <memory>
#include <GLM/glm.hpp>
#include <string>
#include <algorithm>
#include <Gameplay\Transform.h>

using namespace glm;

enum class playerTag {
	PLAYER_ONE,
	PLAYER_TWO
};

class Movement
{

public:
	Movement();
	Movement(Transform::sptr& t) :transform{ t } {};
	Movement(Transform::sptr& t, playerTag p) :transform{ t }, player{ p }{};
	~Movement();
	void setTag(playerTag p) { player = p; };
	void setVelocity(glm::vec3 v) { initialVelocity = v; };
	void setTransform(Transform::sptr& t) { transform = t; };
	void setForce(vec3 f) { force = f; };
	void setDt(float d) { deltaTime = d; };
	void applyForce(vec3 f) { force += f; };
	void setKeyPressed(bool k) { isKeyPressed = k; };
	void movePlayer();
	glm::vec3 getVelocity() { return initialVelocity; };
	glm::vec3 getPosition() { return transform->GetLocalPosition(); };

protected:

	//vec2 for the force of the object, initalized to zero along the x and y
	fvec3 force = vec3(0.0f, 0.0f, 0.0f);

	//Velocity
	fvec3 initialVelocity = vec3(0.0f, 0.0f, 0.0f);

	//float variable that initializes the mass of the puck
	float mass = 20.0f;
	float deltaTime = 0;

	vec3 newVelocity = vec3(0.0f, 0.0f, 0.0f);
	
	//friction variable 
	const float friction = 0.98f;


	//Bool variable that checks 
	bool isKeyPressed = false;
	vec3 initialAcceleration = vec3(0, 0, 0);
	vec3 Magnitude = vec3(0, 0, 0);
	Transform::sptr transform;

	playerTag player;

};
