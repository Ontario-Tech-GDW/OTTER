#pragma once
#include "IBehaviour.h"
#include <vector>
#include <GLM/glm.hpp>

class FollowPathBehaviour final : public IBehaviour
{
public:
	FollowPathBehaviour() :
		Points(std::vector<glm::vec3>()),
		Speed(1.0f),
		_nextPointIx(0) { }
	~FollowPathBehaviour() override = default;

	std::vector<glm::vec3> Points;
	float                  Speed;

	void Update(entt::handle entity) override;
	
private:
	int _nextPointIx;
};