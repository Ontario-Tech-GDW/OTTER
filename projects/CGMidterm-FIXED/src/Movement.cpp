#include "Movement.h"

//https://gamedev.stackexchange.com/questions/4673/how-do-i-implement-deceleration

Movement::Movement()
{
}

Movement::~Movement()
{
}


/*Referneces used for calculating friction and deceleration on the object
* https://answers.unity.com/questions/29751/gradually-moving-an-object-up-to-speed-rather-then.html
* http://www.cplusplus.com/forum/general/29835/
*/

void Movement::movePlayer() {


	//Calculation is done for acceleration
	initialAcceleration = force / mass;

	//Calculate velocity
	newVelocity = initialVelocity + (initialAcceleration * deltaTime);

	//Deceleration Check Logic
	if (isKeyPressed == false)
	{

	////If the user let's go of the keys, a forc of friction is applied 
	////On the paddles and is multiplied by the current force.
	////By doing this, the force slowly decreases to zero, and thus causes the paddles to decelerate
	force.x *= friction;
	force.y *= friction;

	newVelocity.x *= friction;
	newVelocity.y *= friction;

	}


	//Calculates the position
	glm::vec3 position = transform->GetLocalPosition() + (glm::vec3(newVelocity.x, newVelocity.y, newVelocity.z) * deltaTime) + (glm::vec3(initialAcceleration.x, initialAcceleration.y, initialAcceleration.z) * (0.5f) * (deltaTime * deltaTime));


	if (player == playerTag::PLAYER_ONE) {

	if (( position.y > 4.505)|| (position.y < -4.71)||(position.x > 12.63f) || (position.x <1.0f)){

		position = glm::vec3(std::clamp(position.x, 1.0f, 12.63f), std::clamp(position.y, -4.710f, 4.505f), position.z);

		force = glm::vec3(0,0,0);
		newVelocity = glm::vec3(0,0,0);

	}

	}
	else if (player == playerTag::PLAYER_TWO) {

	if ((position.y > 4.505) || (position.y < -4.71) || (position.x < -12.63f) || (position.x > -1.0f)) {

		position = glm::vec3(std::clamp(position.x, -12.623f, -1.0f), std::clamp(position.y, -4.710f, 4.505f), position.z);
		newVelocity = glm::vec3(0,0,0);

	}

	}

	position.z = 4.55;
	//set the local position of the second 
	transform->SetLocalPosition(position.x, position.y, position.z);

	initialVelocity = newVelocity;

}

