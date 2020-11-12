#pragma once
#pragma once

#include <memory>
#include "GameObject.h"
#include "Gameplay/ScoreUI.h"
#include <GLM/glm.hpp>
#include <string>
using namespace std;

class CollisionDetection : public ScoreUI
{

public:
	static bool CheckSphereCollision(GameObject& one, GameObject& two); //Sphere - Sphere collision
	static objectTag CheckWallCollision(GameObject& puck);
};





