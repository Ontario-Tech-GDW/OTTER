#include "ScoreUI.h"
#include "Utilities/ObjLoader.h"

//Constructor
ScoreUI::ScoreUI()
{

}
//Destructor
ScoreUI::~ScoreUI()
{

}

void ScoreUI::updateScore() {

	if (totalScore < 5) {
		totalScore += 1;
	}
	else {
		totalScore = 0;
	}

}
