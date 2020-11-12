#include "Puck.h"

Puck::Puck()
{
}

Puck::Puck(Transform::sptr& t, float m){

transform = t;
mass = 5;
}

Puck::~Puck()
{
}

void Puck::movePuck()
{


	//Calculation is done for acceleration
	initialAcceleration = force / mass;

	//Calculate velocity
	newVelocity = initialVelocity + (initialAcceleration * deltaTime);

	force.x *= friction;
	force.y *= friction;

	newVelocity.x *= friction;
	newVelocity.y *= friction;

	//Calculates the position
	glm::vec3 position = transform->GetLocalPosition() + (glm::vec3(newVelocity.x, newVelocity.y, newVelocity.z) * deltaTime) + (glm::vec3(initialAcceleration.x, initialAcceleration.y, initialAcceleration.z) * (0.5f) * (deltaTime * deltaTime));

	//position.z = 4.55f;

	

	//set the local position of the second 
	transform->SetLocalPosition(position.x, position.y, position.z);

	//Initial Velocity is set to the new velocity
	initialVelocity = newVelocity;

}

void Puck::respawnPuck(playerTag p)
{
	if (p== playerTag::PLAYER_ONE){
		transform->SetLocalPosition(1.0f, 0.0f, 4.45f);
		initialVelocity = glm::vec3 (0.0f, 0.0f, 0.0f);
		force = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else if (p == playerTag::PLAYER_TWO) {
		
		transform->SetLocalPosition(-1.0f, 0.0f, 4.45f);
		initialVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		force = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	
}