#pragma once

#include <memory>
#include <GLM/glm.hpp>
#include "Graphics/VertexArrayObject.h"
#include <Gameplay\Transform.h>

class ScoreUI
{
public:

	ScoreUI();
	~ScoreUI();

	//Getter's and setters to Grab VAO's
	void updateScore();
	void ScoreUI::setTotalScore(int total){ totalScore = total;};
	int ScoreUI::getTotalScore() {return totalScore;};
	Transform::sptr getCurrentTransform() { return transform; };

private:

	//Shared pointer of VAO class is created called 'vao'.
	Transform::sptr transform;
	int totalScore = 0;

};

