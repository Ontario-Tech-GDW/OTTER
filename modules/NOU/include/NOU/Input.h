/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

Input.h
Simple utility class for managing basic keyboard input.
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

namespace nou
{
	class Input
	{
		public:

		~Input() = default;

		static void GLFWInputCallback(GLFWwindow* win, int key, int scancode, int action, int mods);

		static void Init();
		static void FrameStart();

		//Returns true if the key is currently pressed.
		static bool GetKey(int keycode);
		//Returns true if the key was pressed down this frame.
		static bool GetKeyDown(int keycode);
		//Returns true if the key was released this frame.
		static bool GetKeyUp(int keycode);

		protected:

		static const int MAX_KEYS = GLFW_KEY_LAST + 1;

		enum class KeyFlag
		{
			PASSIVE = 0,
			PRESSED,
			RELEASED
		};

		//Instantiating this class doesn't make sense, since all our functionality
		//is exposed statically.
		Input() = default;

		//Stores whether a key is down or not.
		static bool m_keyStates[MAX_KEYS];

		//Stores any message about what happened to the key this frame (i.e., pressed or released)
		static KeyFlag m_keyFlags[MAX_KEYS];
	};
}