#include "Puck.h"

Puck::Puck()
{
}

//Set values for the transform and the mass of the puck
Puck::Puck(Transform::sptr& t, float m){

transform = t;
mass = 5.0f;
}

Puck::~Puck()
{
}


/*

https://gamedev.stackexchange.com/questions/4673/how-do-i-implement-deceleration
https://answers.unity.com/questions/29751/gradually-moving-an-object-up-to-speed-rather-then.html
http://www.cplusplus.com/forum/general/29835/

Referneces used for calculating friction and deceleration on the object

Also Referenced Hello World code from Year One with Hogue

*/



void Puck::movePuck()
{


	//Calculation is done for acceleration
	initialAcceleration = force / mass;

	//Calculate velocity
	newVelocity = initialVelocity + (initialAcceleration * deltaTime);

	//Friction calculations for the puck
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

//respawnPuck function sets the postion of the puck after a goal was scored
void Puck::respawnPuck(playerTag p)
{
	if (p== playerTag::PLAYER_ONE){
		transform->SetLocalPosition(2.0f, 0.0f, 4.45f);
		initialVelocity = glm::vec3 (0.0f, 0.0f, 0.0f);
		force = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else if (p == playerTag::PLAYER_TWO) {
		
		transform->SetLocalPosition(-2.0f, 0.0f, 4.45f);
		initialVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
		force = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	
}