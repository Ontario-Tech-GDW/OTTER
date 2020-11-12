#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::GameObject(glm::vec3 p, objectTag t)
{
	position = p;
	item = t;

	if (item == objectTag::BM_WALL || item ==objectTag::T_WALL)
	{
		size = glm::vec2(31.4, 21.7);
	}
	else if (item == objectTag::LS_WALL || item == objectTag::RS_WALL)
	{
		size = glm::vec2(2, 11.3);

	}else{
		
		size = glm::vec2(2.05, 2.05);
		radius = 1.025f;
	}
}