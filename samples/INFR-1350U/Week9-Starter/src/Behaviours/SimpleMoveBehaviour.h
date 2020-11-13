#pragma once
#include "Gameplay/IBehaviour.h"


class SimpleMoveBehaviour : public IBehaviour
{
public:
	bool Relative = true;
	SimpleMoveBehaviour() = default;
	~SimpleMoveBehaviour() = default;

	void Update(entt::handle entity) override;
};
