/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

App.h
Minimal utility class for GLFW window management.
*/

#pragma once

//This seems silly (and to an extent it is) - but this prevents VS
//from throwing a macro redefinition error at us, since we need this define
//elsewhere.
//P.S. This tells GLFW not to include GL, which is important since GLAD
//will complain if we let anything else load GL by itself.
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"

#include <string>

namespace nou
{
	class App
	{
		public:

		~App() = default;

		static void Init(const std::string& name, int width, int height);
		static void InitImgui();
		static void Cleanup();

		static void Tick();
		static void FrameStart();
		static void SwapBuffers();

		static void StartImgui();
		static void EndImgui();

		static float GetDeltaTime();
		static bool IsClosing();

		static void SetClearColor(const glm::vec4& clearColor);

		protected:

		//Instantiating this class doesn't make sense, since all our functionality
		//is exposed statically.
		App() = default;

		static GLFWwindow* m_window;
		static float m_prevTime;
		static float m_deltaTime;
		static bool m_imguiInit;
	};
}