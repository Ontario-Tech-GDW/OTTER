#pragma once
#include <GLM/glm.hpp>
#include <time.h>
namespace Util
{
	

	bool Init();

	bool CheckNumBetween(int num, int min, int max);
	bool CheckNumBetween(float num, float min, float max);
	bool CheckNumBetween(glm::vec2 num, glm::vec2 min, glm::vec2 max);
	bool CheckNumBetween(glm::vec3 num, glm::vec3 min, glm::vec3 max);
	bool CheckNumBetween(glm::vec4 num, glm::vec4 min, glm::vec4 max);

	int GetRandomNumberBetween(int from, int to, int avoidFrom = 0.0f, int avoidTo = 0.0f);
	float GetRandomNumberBetween(float from, float to, float avoidFrom = 0.0f, float avoidTo = 0.0f);
	glm::vec2 GetRandomNumberBetween(glm::vec2 from, glm::vec2 to, glm::vec2 avoidFrom = glm::vec2(), glm::vec2 avoidTo = glm::vec2());
	glm::vec3 GetRandomNumberBetween(glm::vec3 from, glm::vec3 to, glm::vec3 avoidFrom = glm::vec3(), glm::vec3 avoidTo = glm::vec3());
	glm::vec3 GetRandomNumberBetween(glm::vec4 from, glm::vec4 to, glm::vec4 avoidFrom = glm::vec4(), glm::vec4 avoidTo = glm::vec4());
}