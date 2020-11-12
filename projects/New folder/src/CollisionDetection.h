#pragma once
#pragma once

#include <memory>
#include <string>
#include <algorithm>
#include "GameObject.h"
#include "Gameplay/ScoreUI.h"
#include <GLM/glm.hpp>
#include "Gameplay/ScoreUI.h"
#include "Puck.h"

class CollisionDetection : public ScoreUI
{

public:
	static bool CheckSphereCollision(GameObject& one, GameObject& two); //Sphere - Sphere collision
	static objectTag CheckWallCollision(GameObject& puck, Puck p);
};





