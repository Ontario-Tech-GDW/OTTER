#pragma once
#include "GLFW/glfw3.h"

// We can declare the name and assume it will get included later, helps avoid circular dependencies
class GameScene;

class Application
{
public:
	static Application& Instance()
	{
		static Application instance;
		return instance;
	}

	GLFWwindow* Window;
	std::shared_ptr<GameScene> ActiveScene;
};
